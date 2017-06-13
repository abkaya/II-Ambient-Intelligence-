import json
import datetime
import os
from sys import argv
import time
import paho.mqtt.client as mqtt
import division
import PIPE, Popen

def get_cpu_temperature():
    process = Popen(['vcgencmd', 'measure_temp'], stdout=PIPE)
    output, _error = process.communicate()
    return float(output[output.index('=') + 1:output.rindex("'")])

# CLIENT INFO
clientid = "Willem-develop6"
clientprotocol = 3  # MQTTv3.1
mqttc = mqtt.Client(client_id=clientid, protocol=clientprotocol)

# MQTT SERVER INFO
MQTT_server = "localhost"
MQTT_topic_pub = "/rpi/system/temp"

while True:

    mqttc.connect(MQTT_server, 1883, 60)
    mqttc.publish(MQTT_topic_pub, get_cpu_temperature())
    mqttc.disconnect()
    time.sleep(10)