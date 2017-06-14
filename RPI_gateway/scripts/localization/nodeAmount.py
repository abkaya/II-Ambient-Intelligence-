import json
import os
from sys import argv

import paho.mqtt.client as mqtt

prompt = '> '

# Node ID:
# Willem --> "node": "b57000009128e"
# Frederik --> "node": "b57000009127b"
# Michiel --> "node": "b5700000913b8"

room_name = "V315"
nodes_found = []


def on_message(mqttc, obj, msg):
    # MESSAGE TO JSON

    payload = msg.payload.replace("'",'"')
    payload_JSON = json.loads(payload)

    if(payload_JSON["room_name"] == room_name):
        global sleepcount
        sleepcount = 0
        if payload_JSON["node_id"] not in nodes_found:
            nodes_found.append(payload_JSON["node_id"])
        global count
        count = count + 1

# CLIENT INFO
clientid = "Willem-develop2"
clientprotocol = 3  # MQTTv3.1
mqttc = mqtt.Client(client_id=clientid, protocol=clientprotocol)

# SETUP METHODS
mqttc.on_message = on_message

# MQTT SERVER INFO
MQTT_server = "localhost"
MQTT_topic_sub = "/rpi/localization/node"
MQTT_topic_pub = "/rpi/localization/amount"

# Create connection
print "Connect"
mqttc.connect(MQTT_server, 1883, 60)
mqttc.subscribe(MQTT_topic_sub)

while True:
    del nodes_found[:]
    count = 0
    sleepcount = 0
    while count < 4:
        sleepcount = sleepcount + 1
        mqttc.loop()
        if(sleepcount > 5):
            del nodes_found[:]
            mqttc.publish(MQTT_topic_pub,0)
            print 0
            sleepcount = 0
    mqttc.publish(MQTT_topic_pub,len(nodes_found))
    print len(nodes_found)