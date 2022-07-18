from pid import PidController
from pid_state_model import (
    PIDStateModel,
    create_gauges as create_state_gauges,
    update_gauges as update_state_gauges,
)
from pid_config_model import (
    PIDConfigModel,
    create_gauges as create_config_gauges,
    update_gauges as update_config_gauges,
)
from config_model import ConfigModel
from output_controllers import *
from temp_providers import *
import time
import json
import zmq
from stopwatch import Stopwatch
from data_logger import LoggerMgr, ConsoleColor
from db_manager import DbManager
import os
import shutil
from mqtt_client import MqttClient
from prometheus_client import start_http_server, Gauge
import requests
import threading

state_gauges = create_state_gauges(Gauge)
config_gauges = create_config_gauges(Gauge)


def test_main():
    pid_config = PIDConfigModel.convert_json_data_to_model({})
    pid_state = PIDStateModel.convert_json_data_to_model({})
    pid = PidController()
    output_controller = MockOutputController()
    oven_temp_provider = MockTempProvider()

    for i in range(1, 100000):

        pid.pid_loop(
            1, pid_config, pid_state, oven_temp_provider, [], output_controller
        )
        if pid_state.output > 0:
            oven_temp_provider.temp = oven_temp_provider.temp + 1
        else:
            oven_temp_provider.temp = oven_temp_provider.temp - 1
        time.sleep(0.1)


class SetPointUpdater:
    def __init__(self, ha_token):
        self.ha_token = ha_token
        self.set_point = 0
        self.session = requests.Session()

    def start(self):
        server_thread = threading.Thread(target=self.updater_thread)
        server_thread.start()

    def updater_thread(self):
        while True:
            try:
                r = self.session.get(
                    "https://homeassistant.savage.zone/api/states/input_number.bbq_temperature_set_point",
                    headers={"Authorization": f"Bearer {self.ha_token}"},
                )
                new_set_point = int(float(r.json()["state"]))
                self.set_point = new_set_point
            except BaseException as e:
                LoggerMgr.warning(str(e))
            time.sleep(15)



class ServerApplication(object):
    MQTT_TOPIC_SET_TEMP = "bbq_pit/heat/temperature/set"

    def __init__(
        self,
        config_file,
        temp_provider_factory,
        output_controller_factory,
        pid_controller,
    ):
        self.pid_state_model = PIDStateModel.convert_json_data_to_model({})
        self.config_file = config_file
        self.config_model = ConfigModel.load_from_file(self.config_file)
        self.config_model.save_to_file(self.config_file)
        for key in self.config_model.env_config:
            os.environ[key] = self.config_model.env_config[key]
        LoggerMgr.configure(self.config_model.logging)
        self.oven_temp_provider = temp_provider_factory.CreateTempProvider(
            self.config_model.io_config.oven_temp_provider, "Oven"
        )
        self.opt_temp_providers = []
        for opt_temp_provider_entry in self.config_model.io_config.opt_temp_providers:
            self.opt_temp_providers.append(
                temp_provider_factory.CreateTempProvider(
                    opt_temp_provider_entry["type"], opt_temp_provider_entry["label"]
                )
            )
        self.output_controller = output_controller_factory.CreateOutputController(
            self.config_model.io_config.output_controller
        )

        self.pid = pid_controller
        self.pid_interval = 1.0

        # create zmq server
        ctx = zmq.Context.instance()
        self.socket = ctx.socket(zmq.REP)
        self.socket.bind(self.config_model.io_config.server_address)
        self.poller = zmq.Poller()
        self.poller.register(self.socket, zmq.POLLIN)

        # db connector
        # self.db_engine = DbManager.create_db_engine()

        self.mqtt_client = MqttClient("bbq/status", "mosquitto.savage.zone", 1883, subscribe_callback=self.on_mqtt_message)
        self.mqtt_client.subscribe(self.MQTT_TOPIC_SET_TEMP)


    def run_pid(self):
        self.pid.pid_iteration(
            self.pid_interval,
            self.config_model.pid_config,
            self.pid_state_model,
            self.oven_temp_provider,
            self.opt_temp_providers,
            self.output_controller,
        )

    def poll_message(self):
        ready_sockets = dict(self.poller.poll(self.pid_interval * 1000))
        if self.socket in ready_sockets:
            return self.socket.recv_json()
        return None

    def apply_logging_config(self, config_msg):
        if "logging" in config_msg:
            log_config = config_msg["logging"]
            try:
                LoggerMgr.configure(self.config_model.logging)
                self.config_model.logging = log_config
                self.config_model.save_to_file(self.config_file)
            except Exception as e:
                LoggerMgr.warning(str(e))

    def apply_pid_state(self, pid_state_msg):
        new_pid_state = self.pid_state_model.copy()

        # apply pid_state_msg to new_pid_state
        try:
            if new_pid_state.apply(pid_state_msg):
                LoggerMgr.info("*** applying new state!", color=ConsoleColor.OKBLUE)
                self.pid_state_model = new_pid_state
            else:
                LoggerMgr.info("*** nothing to apply!", color=ConsoleColor.OKBLUE)
        except ValueError as e:
            LoggerMgr.warning("******** ValueError: {}".format(str(e)))
            pass

    def apply_pid_config(self, pid_config_msg):
        new_pid_config = self.config_model.pid_config.copy()

        # apply pid_config_msg to new_pid_config
        try:
            if new_pid_config.apply(pid_config_msg):
                LoggerMgr.info(
                    "*** applying new configuration!", color=ConsoleColor.OKBLUE
                )
                self.config_model.pid_config = new_pid_config
                self.config_model.save_to_file(self.config_file)
            else:
                LoggerMgr.info("*** nothing to apply!", color=ConsoleColor.OKBLUE)
        except ValueError as e:
            LoggerMgr.warning("******** ValueError: {}".format(str(e)))
            pass

    def on_mqtt_message(self, message):
        if message.topic == self.MQTT_TOPIC_SET_TEMP:
            try:
                set_point = float(message.payload)
                self.update_set_point(set_point)
            except ValueError as e:
                LoggerMgr.warning(f"******** ValueError: {str(e)}")


    def update_set_point(self, new_set_point):
        if new_set_point != self.config_model.pid_config.set_point:
            pid_config = {"set_point": new_set_point}
            self.apply_pid_config(pid_config)

    def main_loop(self):
        LoggerMgr.info("Starting ServerApplication.main_loop()")
        self.run_pid()
        iteration = 0
        ha_token = os.environ["HA_TOKEN"]
        set_point_updater = SetPointUpdater(ha_token)
        set_point_updater.start()

        while True:
            stopwatch = Stopwatch()
            received_message = self.poll_message()
            if received_message:
                LoggerMgr.info(
                    "*** received message from client", color=ConsoleColor.OKBLUE
                )
                LoggerMgr.info(
                    "*** message = {}".format(received_message),
                    color=ConsoleColor.OKBLUE,
                )
                if "pid_config" in received_message:
                    self.apply_pid_config(received_message["pid_config"])
                if "pid_state" in received_message:
                    self.apply_pid_state(received_message["pid_state"])

            elapsed = stopwatch.elapsed()
            if elapsed < self.pid_interval:
                time.sleep(self.pid_interval - elapsed)

            self.run_pid()

            publish_interval = 5
            if iteration % publish_interval == 0:
                new_set_point = set_point_updater.set_point
                self.update_set_point(new_set_point)

                # publish state to MQTT
                self.mqtt_client.publish(
                    {
                        **self.config_model.pid_config.as_dict(),
                        **self.pid_state_model.as_dict(),
                    }
                )

                # update prometheus gauges
                update_state_gauges(state_gauges, self.pid_state_model)
                update_config_gauges(config_gauges, self.config_model.pid_config)

            iteration += 1

            if received_message is not None:
                # send reply with state
                LoggerMgr.info(
                    "*** sending message back to client", color=ConsoleColor.OKBLUE
                )
                self.socket.send_json(self.pid_state_model.as_dict())


if __name__ == "__main__":
    config_dir = "/config"
    config_file = config_path = "config.json"
    if os.path.isdir(config_dir):
        config_path = os.path.join(config_dir, config_file)
        if not os.path.isfile(config_path):
            shutil.copyfile(config_file, config_path)

    start_http_server(9009)

    app = ServerApplication(
        config_path,
        get_temp_provider_factory(),
        get_output_controller_factory(),
        PidController(),
    )
    app.main_loop()
