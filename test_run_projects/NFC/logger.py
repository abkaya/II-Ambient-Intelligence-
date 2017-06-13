import json
import datetime
import os
from sys import argv
import time
import paho.mqtt.client as mqtt


def on_message(mqttc, obj, msg):
    # MESSAGE TO JSON
    payload = msg.payload
    time = datetime.datetime.now()
    with open("/etc/openhab2/html/NFC/log.txt", "a") as myfile:
        myfile.write(payload + "\t" + str(time))

# CLIENT INFO
clientid = "Willem-develop4"
clientprotocol = 3  # MQTTv3.1
mqttc = mqtt.Client(client_id=clientid, protocol=clientprotocol)

# SETUP METHODS
mqttc.on_message = on_message

# MQTT SERVER INFO
MQTT_server = "localhost"
MQTT_topic_sub = "/rpi/nfc"

# Create connection
print "Connect"
mqttc.connect(MQTT_server, 1883, 60)
mqttc.subscribe(MQTT_topic_sub)

while True:
    mqttc.loop()

