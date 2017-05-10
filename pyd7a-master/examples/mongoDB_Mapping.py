import datetime
import json
import os
import sys
from sys import argv

import paho.mqtt.client as mqtt
from pymongo import MongoClient

import numpy

prompt = '> '

# b5700000912bf
gateway1 = []
# b5700000912fd
gateway2 = []
# b5700000912d9
gateway3 = []
# b570000091ac9
gateway4 = []
# b5700000912d5
gateway5 = []
# b570000091291
gateway6 = []

# Willem --> "node": "b57000009128e"
# Frederik --> "node": "b57000009127b"
# Michiel --> "node": "b5700000913b8"

def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))


def on_message(mqttc, obj, msg):
    # Default incoming message
    #print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))

    # MESSAGE TO JSON
    payload = msg.payload
    payload_JSON = json.loads(payload)

    if(payload_JSON["node"] == nodeid):
        print payload_JSON
        RSSI_value = payload_JSON["link_budget"]
        gatewayID = str(payload_JSON["gateway"])

        if gatewayID == 'b5700000912bf':
            gateway1.append(RSSI_value)
        if gatewayID == 'b5700000912fd':
            gateway2.append(RSSI_value)  
        if gatewayID == 'b5700000912d9':
            gateway3.append(RSSI_value)
        if gatewayID == 'b570000091ac9':
            gateway4.append(RSSI_value)          
        if gatewayID == 'b5700000912d5':
            gateway5.append(RSSI_value)
        if gatewayID == 'b570000091291':
            gateway6.append(RSSI_value)

        global count
        count = count + 1



def on_publish(mqttc, obj, mid):
    print("mid: " + str(mid))


def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_log(mqttc, obj, level, string):
    print(string)


# CLIENT INFO
clientid = "Willem-develop"
clientprotocol = 3  # MQTTv3.1
mqttc = mqtt.Client(client_id=clientid, protocol=clientprotocol)
nodeid = "b57000009128e"

# SETUP METHODS
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
#mqttc.on_log = on_log

# MQTT SERVER INFO
MQTT_server = "backend.idlab.uantwerpen.be"
MQTT_topic = "/localisation/DASH7"

print("Client: " + clientid)
print("Server: " + MQTT_server)
print("Topic: " + MQTT_topic)
print

#Connect to MongoDB
client = MongoClient('localhost', 27017)
db = client['Ambient']
collection = db['V-blok']
  

# CREATE ROOM
print "Give room name:"
room_name = raw_input(prompt)

room_id = 1

while True:
    raw_input(prompt)
    json_data = []
    # Create connection
    print("Create connection....")
    mqttc.connect(MQTT_server, 1883, 60)
    mqttc.subscribe(MQTT_topic)
    print("Connection created....\n")

    count = 0
    while count < 20:
        mqttc.loop()
        
    if len(gateway1) > 0:
        # RSSI_gateway1 = round(sum(gateway1) / float(len(gateway1)))
        a = numpy.array(gateway1)
        RSSI_gateway1 = numpy.median(a)
    if len(gateway2) > 0:
        # RSSI_gateway2 = round(sum(gateway2) / float(len(gateway2)))
        a = numpy.array(gateway2)
        RSSI_gateway2 = numpy.median(a)
    if len(gateway3) > 0:
        # RSSI_gateway3 = round(sum(gateway3) / float(len(gateway3)))
        a = numpy.array(gateway3)
        RSSI_gateway3 = numpy.median(a)
    if len(gateway4) > 0:
        # RSSI_gateway4 = round(sum(gateway4) / float(len(gateway4)))
        a = numpy.array(gateway4)
        RSSI_gateway4 = numpy.median(a)
    if len(gateway5) > 0:
        # RSSI_gateway5 = round(sum(gateway5) / float(len(gateway5)))
        a = numpy.array(gateway5)
        RSSI_gateway5 = numpy.median(a)
    if len(gateway6) > 0:
        # RSSI_gateway6 = round(sum(gateway6) / float(len(gateway6)))
        a = numpy.array(gateway6)
        RSSI_gateway6 = numpy.median(a)

    data = {}
    data['room_id'] = int(room_id)
    data['room_name'] = room_name
    data['data_type'] = "raw_mean"
    timestamp = datetime.datetime.now()
    data['timestamp'] =timestamp
    RSSI = []
    if len(gateway1) > 0:
        RSSI.append({"Gateway": "b5700000912bf", "RSSI-Value": RSSI_gateway1})
    else:
        RSSI.append({"Gateway": "b5700000912bf", "RSSI-Value": 0})
    if len(gateway2) > 0:
        RSSI.append({"Gateway": "b5700000912fd", "RSSI-Value": RSSI_gateway2})
    else:
        RSSI.append({"Gateway": "b5700000912fd", "RSSI-Value": 0})
    if len(gateway3) > 0:
        RSSI.append({"Gateway": "b5700000912d9", "RSSI-Value": RSSI_gateway3})
    else:
        RSSI.append({"Gateway": "b5700000912d9", "RSSI-Value": 0})
    if len(gateway4) > 0:
        RSSI.append({"Gateway": "b570000091ac9", "RSSI-Value": RSSI_gateway4})
    else:
        RSSI.append({"Gateway": "b570000091ac9", "RSSI-Value": 0})
    if len(gateway5) > 0:
        RSSI.append({"Gateway": "b5700000912d5", "RSSI-Value": RSSI_gateway5})
    else:
        RSSI.append({"Gateway": "b5700000912d5", "RSSI-Value": 0})
    if len(gateway6) > 0:
        RSSI.append({"Gateway": "b570000091291", "RSSI-Value": RSSI_gateway6})
    else:
        RSSI.append({"Gateway": "b570000091291", "RSSI-Value": 0})
    data['RSSI'] = RSSI
    collection.insert(data)
    room_id = room_id + 1
