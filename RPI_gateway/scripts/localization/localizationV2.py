import json
import math
import operator
import time
from shutil import copyfile
from sys import argv

import numpy
import paho.mqtt.client as mqtt
from pymongo import MongoClient
prompt = '> '

# MOBILE NODES
# Willem --> "node": "b57000009128e"
# Frederik --> "node": "b57000009127b"
# Michiel --> "node": "b5700000913b8"
nodeid = "b57000009128e"

# GATEWAYS
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

# Static variables
maxMapMessages = 10
k = 5

# Connections
clientid = "Willem-develop"
group_name = "Team-Cool"
collection_name = "V-Blok"
MQTT_server = "backend.idlab.uantwerpen.be"  # Dont't change this
MQTT_topic = "/localisation/DASH7"  # Dont't change this
MQTT_RPi = "localhost"  # Dont't change this
MQTT_topic_RPi = "/rpi/localization"  # Dont't change this

# MQTT on_message
def on_message(mqttc1, obj, msg):

    # MESSAGE TO JSON
    payload = msg.payload
    payload_JSON = json.loads(payload)

    # Filter on node
    if (payload_JSON["node"] == nodeid):
        #print payload_JSON
        RSSI_value = payload_JSON["link_budget"]
        gatewayID = str(payload_JSON["gateway"])

        # Filter on gateway
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

        # Counter for max messages
        global count
        count = count + 1

        if(count >= maxMapMessages):
            fingerprinting()
            count = 0
            clearGateways()
# Clear gateway data
def clearGateways():
    del gateway1[:]
    del gateway2[:]
    del gateway3[:]
    del gateway4[:]
    del gateway5[:]
    del gateway6[:]

# kNN algorithm - euclidian
def euclideanDistance(instance1, instance2, length):
    distance = 0
    for x in range(length):
        distance += pow((instance1[x] - instance2[x]), 2)
    return math.sqrt(distance)

# kNN algorithm - manhatten
def manhattenDistance(instance1, instance2, length):
    distance = 0
    for x in range(length):
        distance += abs(instance1[x] - instance2[x])
    return distance

# kNN algorithm - get neighbors of mobile node
def getNeighbors(trainingSet, testInstance, k):
    distances = []
    length = len(testInstance) - 1
    for x in range(len(trainingSet)):
        dist = euclideanDistance(testInstance, trainingSet[x], length)
        distances.append((trainingSet[x], dist))
    distances.sort(key=operator.itemgetter(1))
    neighbors = []
    for x in range(k):
        print "Neighbour" + str(x) + ":"
        print distances[x][0][6] + "_" + str(distances[x][0][7]) + "_" + str(distances[x][0][8])
        neighbors.append(distances[x][0])
    return neighbors


# kNN algorithm - fingerprinting
previous_room = ""


def fingerprinting():

    # Get TrainingData
    d = []
    cursor = collection.find({})
    for document in cursor:
        d.append(document)
    trainSet = []
    for fi in d:
        finger = []
        room_name = fi["room_name"]
        box = fi["boxid"]
        fingerid = fi["fingerid"]
        RSSI_array = fi["RSSI"]
        for RSSI in RSSI_array:
            finger.append(RSSI["RSSI-Value"])
        finger.append(room_name)
        finger.append(box)
        finger.append(fingerid)
        trainSet.append(finger)

    # Get TestData
    testInstance = []
    if len(gateway1) > 0:
        a = numpy.array(gateway1)
        testInstance.append(numpy.median(a))
    else:
        testInstance.append(0)
    if len(gateway2) > 0:
        a = numpy.array(gateway2)
        testInstance.append(numpy.median(a))
    else:
        testInstance.append(0)
    if len(gateway3) > 0:
        a = numpy.array(gateway3)
        testInstance.append(numpy.median(a))
    else:
        testInstance.append(0)
    if len(gateway4) > 0:
        a = numpy.array(gateway4)
        testInstance.append(numpy.median(a))
    else:
        testInstance.append(0)
    if len(gateway5) > 0:
        a = numpy.array(gateway5)
        testInstance.append(numpy.median(a))
    else:
        testInstance.append(0)
    if len(gateway6) > 0:
        a = numpy.array(gateway6)
        testInstance.append(numpy.median(a))
    else:
        testInstance.append(0)

    neighbors = getNeighbors(trainSet, testInstance, k)
    neighborsRoom = []
    checkListRoom = []
    checkListBox = []
    for x in range(len(neighbors)):
        if(neighbors[x][6] not in checkListRoom):
            checkListRoom.append(neighbors[x])
        else:
            checkListRoom[checkListRoom.index(neighbors[x][6])].append(neighbors[x])    
    
    for x in range(len(checkListRoom)):
        if(checkListRoom[x][7] not in checkListBox):
            checkListBox.append(neighbors[x])
        else:
            checkListBox[checkListBox.index(neighbors[x][7])].append(checkListRoom[x])

    current_room = str(checkListBox[0][6]) + "_" + str(checkListBox[0][7])
    global previous_room
    
    if(previous_room != current_room):
        print "Room: " + current_room
        print
        # json_loc = {"node_id": nodeid, "room_name": current_room}  
        # mqttc2.connect(MQTT_RPi, 1883, 60)
        # mqttc2.publish(MQTT_topic_RPi, current_room)
        # mqttc2.disconnect()
        # copyfile('/home/pi/localization/images/' + current_room + '.png', '/etc/openhab2/html/localization/current.png')
        # bashCommand="sudo convert /etc/openhab2/html/localization/current.png -pointsize 18 -fill #00CCFF -annotate +600+50 " +  'Current Room: ' +current_room+ " /etc/openhab2/html/localization/current.png"
        # process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
        # output, error = process.communicate()
        # previous_room = current_room
    
# Connect to MongoDB
client = MongoClient('localhost', 27017)
db = client[group_name]
collection = db[collection_name]

# MQTT - SERVER INFO
mqttc1 = mqtt.Client(client_id=clientid, protocol=3)
mqttc2 = mqtt.Client(client_id=clientid, protocol=3)

# MQTT - SETUP METHODS
mqttc1.on_message = on_message

# MQTT - SERVER INFO
print("Client: " + clientid)
print("Server: " + MQTT_server)
print("Topic: " + MQTT_topic)
print

# MQTT - RPI INFO
print("Client: " + clientid)
print("Server: " + MQTT_RPi)
print("Topic: " + MQTT_topic_RPi)
print

# MQTT - Create connection with server and subscribe/RPI publisch connect
print("Create connections....")
mqttc1.connect(MQTT_server, 1883, 60)
mqttc1.subscribe(MQTT_topic)
print("Connection created....\n")

count = 0
# Program loop
while True:
    mqttc1.loop()
