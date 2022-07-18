from paho.mqtt.client import Client
from data_logger import LoggerMgr
import json


class MqttClient:
    def __init__(self, default_publish_topic, server, port, username=None, password=None, subscribe_callback=None):
        self.connected = False
        self.subscribe_callback = subscribe_callback
        self.default_publish_topic = default_publish_topic
        self.client = Client()
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        if username is not None and password is not None:
            self.client.username_pw_set(username, password)
        self.client.connect(server, port)
        self.client.loop_start()
        self.client.on_message = self.on_message

    def on_connect(self, client, userdata, flags, rc):
        LoggerMgr.info("Connected to MQTT Server")
        self.connected = True

    def on_disconnect(self, client, userdata, rc):
        self.connected = False

    def on_message(self, client, userdata, message):
        if self.subscribe_callback is not None:
            self.subscribe_callback(message)

    def publish(self, data, topic=None):
        if self.connected:
            self.client.publish(topic or self.default_publish_topic, payload=json.dumps(data))

    def subscribe(self, topic, qos=0):
        self.client.subscribe(topic, qos)

