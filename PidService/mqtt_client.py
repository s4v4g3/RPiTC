from paho.mqtt.client import Client
from data_logger import LoggerMgr
import json




class MqttClient():
    def __init__(self, topic, server, port, username=None, password=None):
        self.connected = False
        self.topic = topic
        self.client = Client()
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        if username is not None and password is not None:
            self.client.username_pw_set(username, password)
        self.client.connect(server)

    def on_connect(self, client, userdata, flags, rc):
        LoggerMgr.info("Connected to MQTT Server")
        self.connected = True

    def on_disconnect(self, client, userdata, rc):
        self.connected = False

    def publish(self, data):
        if self.connected:
            self.client.publish(self.topic, payload=json.dumps(data))