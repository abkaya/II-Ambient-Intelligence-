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

def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))

def on_message(mqttc, obj, msg):
    # MESSAGE TO JSON
    payload = msg.payload.replace("'",'"')
    print payload
    payload_JSON = json.loads(payload)
    print payload_JSON
    if(payload_JSON["room_name"] == room_name):
        if payload_JSON["node_id"] not in nodes_found:
            nodes_found.append(payload_JSON["node_id"])
        global count
        count = count + 1

def on_publish(mqttc, obj, mid):
    print("mid: " + str(mid))


def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_log(mqttc, obj, level, string):
    print(string)


# CLIENT INFO
clientid = "Willem-develop2"
clientprotocol = 3  # MQTTv3.1
mqttc = mqtt.Client(client_id=clientid, protocol=clientprotocol)

# SETUP METHODS
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
#mqttc.on_log = on_log

# MQTT SERVER INFO
MQTT_server = "143.129.37.80"
MQTT_topic_sub = "/rpi/localization/node"
MQTT_topic_pub = "/rpi/localization/amount"

print("Client: " + clientid)
print("Server: " + MQTT_server)
print("Topic: " + MQTT_topic_sub)
print

# Create connection
print("Create connection....")
mqttc.connect(MQTT_server, 1883, 60)
mqttc.subscribe(MQTT_topic_sub)
print("Connection created....\n")

while True:
    del nodes_found[:]
    count = 0
    while count < 6:
        mqttc.loop()
        
    mqttc.publish(MQTT_topic_pub,len(nodes_found))
