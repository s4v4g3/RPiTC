from pid import PidController
from pid_state_model import PIDStateModel
from pid_config_model import PIDConfigModel
from config_model import ConfigModel
from output_controllers import *
from temp_providers import *
import time
import json
import zmq
from stopwatch import Stopwatch
from data_logger import LoggerMgr, ConsoleColor
from db_manager import DbManager


def test_main():
    pid_config = PIDConfigModel.convert_json_data_to_model({})
    pid_state = PIDStateModel.convert_json_data_to_model({})
    pid = PidController()
    output_controller = MockOutputController()
    oven_temp_provider = MockTempProvider()

    for i in range(1, 100000):

        pid.pid_loop(1, pid_config, pid_state, oven_temp_provider, [], output_controller)
        if pid_state.output > 0:
            oven_temp_provider.temp = oven_temp_provider.temp + 1
        else:
            oven_temp_provider.temp = oven_temp_provider.temp - 1
        time.sleep(0.1)

class ServerApplication(object):

    def __init__(self, config_file, temp_provider_factory, output_controller_factory, pid_controller):
        self.pid_state_model = PIDStateModel.convert_json_data_to_model({})
        self.config_file = config_file
        self.config_model = ConfigModel.load_from_file(self.config_file)
        self.config_model.save_to_file(self.config_file)
        LoggerMgr.configure(self.config_model.logging)
        self.oven_temp_provider = temp_provider_factory.CreateTempProvider(self.config_model.io_config.oven_temp_provider, 'Oven')
        self.opt_temp_providers =[]
        for opt_temp_provider_entry in self.config_model.io_config.opt_temp_providers:
            self.opt_temp_providers.append(temp_provider_factory.CreateTempProvider(opt_temp_provider_entry['type'],
                                           opt_temp_provider_entry['label']))
        self.output_controller = output_controller_factory.CreateOutputController(self.config_model.io_config.output_controller)

        self.pid = pid_controller
        self.pid_interval = 1.0

        # create zmq server
        ctx = zmq.Context.instance()
        self.socket = ctx.socket(zmq.REP)
        self.socket.bind(self.config_model.io_config.server_address)
        self.poller = zmq.Poller()
        self.poller.register(self.socket, zmq.POLLIN)

        # db connector
        self.db_engine = DbManager.create_db_engine()

    def run_pid(self):
        self.pid.pid_iteration(self.pid_interval, self.config_model.pid_config, self.pid_state_model, self.oven_temp_provider, self.opt_temp_providers,
                 self.output_controller)

    def poll_message(self):
        ready_sockets = dict(self.poller.poll(self.pid_interval * 1000))
        if self.socket in ready_sockets:
            return self.socket.recv_json()
        return None

    def apply_logging_config(self, config_msg):
        if 'logging' in config_msg:
            log_config = config_msg['logging']
            try:
                LoggerMgr.configure(self.config_model.logging)
                self.config_model.logging = log_config
                self.config_model.save_to_file(self.config_file)
            except Exception as e:
                LoggerMgr.warning(str(e))


    def apply_pid_config(self, config_msg):
        if 'pid_config' in config_msg:
            pid_config_msg = config_msg['pid_config']
            new_pid_config = self.config_model.pid_config.copy()

            # apply pid_config_msg to new_pid_config
            try:
                if new_pid_config.apply(pid_config_msg):
                    LoggerMgr.info("*** applying new configuration!", color=ConsoleColor.OKBLUE)
                    self.config_model.pid_config = new_pid_config
                    self.config_model.save_to_file(self.config_file)
                else:
                    LoggerMgr.info("*** nothing to apply!", color=ConsoleColor.OKBLUE)
            except ValueError as e:
                LoggerMgr.warning("******** ValueError: {}".format(str(e)))
                pass

    def main_loop(self):
        LoggerMgr.info("Starting ServerApplication.main_loop()")
        self.run_pid()
        iteration = 0
        while(True):
            stopwatch = Stopwatch()
            received_message = self.poll_message()
            if received_message:
                LoggerMgr.info("*** received message from client", color=ConsoleColor.OKBLUE)
                LoggerMgr.info("*** message = {}".format(received_message), color=ConsoleColor.OKBLUE)
                self.apply_pid_config(received_message)
                if 'pid_config' in received_message:
                    self.apply_pid_config(received_message['pid_config'])


            elapsed = stopwatch.elapsed()
            if elapsed < self.pid_interval:
                time.sleep(self.pid_interval - elapsed)


            self.run_pid()

            db_insert_interval = 15
            if iteration % db_insert_interval == 0:
                self.db_engine.insert(self.config_model.pid_config, self.pid_state_model)
            iteration += 1

            if received_message is not None:
                # send reply with state
                LoggerMgr.info("*** sending message back to client", color=ConsoleColor.OKBLUE)
                self.socket.send_json(self.pid_state_model.as_dict())


if __name__ == "__main__":
    app = ServerApplication("config.json", get_temp_provider_factory(), get_output_controller_factory(),
                            PidController())
    app.main_loop()