import json
import math
import operator
import time
from shutil import copyfile
from sys import argv

import numpy
import paho.mqtt.client as mqtt
from pymongo import MongoClient

import argparse

import time
import ast

prompt = '> '

# Variables
mag_valueX = []
mag_valueY = []
mag_valueZ = []
pitchs = []
rolls = []
yaws = []
bars = []

# Static variables
maxMapMessages = 1
k = 5

# Connections
clientid = "Willem-develop"
group_name = "Team-MetVier"
collection_name = "V-Blok_mag_0"
MQTT_RPi = "localhost"
MQTT_topic_server = "/d7node/localization"
MQTT_topic_RPi = "/rpi/localization_mag/node"

# MQTT on_message
def on_message(mqttc1, obj, msg):
    dataLocal = ast.literal_eval(msg.payload)
    dataHeader = int(format(dataLocal[0], '#010b'), 0)
    #print dataHeader
    # Only process data if first byte equals '42' (= data header for mobile
    # node). This prevents conflicts with the NFC node.
    if(dataHeader == 42):
        #print dataLocal
        mag_valueX.append(
            int(format(dataLocal[1], '#010b') + format(dataLocal[2], '08b'), 2))
        mag_valueY.append(
            int(format(dataLocal[3], '#010b') + format(dataLocal[4], '08b'), 2))
        mag_valueZ.append(
            int(format(dataLocal[5], '#010b') + format(dataLocal[6], '08b'), 2))
        pitchs.append(
            int(format(dataLocal[7], '#010b') + format(dataLocal[8], '08b'), 2))
        rolls.append(
            int(format(dataLocal[9], '#010b') + format(dataLocal[10], '08b'), 2))
        yaws.append(
            int(format(dataLocal[11], '#010b') + format(dataLocal[12], '08b'), 2))
        bars.append(
            int(format(dataLocal[13], '#010b') + format(dataLocal[14], '08b'), 2))

        # Counter for max messages
        global count
        count = count + 1

        if (count >= maxMapMessages):
            fingerprinting()
            count = 0
            clearVariables()


# def received_command_callback(cmd):
#     dataLocal = cmd.actions[0].operand.data
#     dataHeader = int(format(dataLocal[0], '#010b'), 0)
#     #print dataHeader
#     # Only process data if first byte equals '42' (= data header for mobile
#     # node). This prevents conflicts with the NFC node.
#     if(dataHeader == 42):
#         #print dataLocal
#         mag_valueX.append(
#             int(format(dataLocal[1], '#010b') + format(dataLocal[2], '08b'), 2))
#         mag_valueY.append(
#             int(format(dataLocal[3], '#010b') + format(dataLocal[4], '08b'), 2))
#         mag_valueZ.append(
#             int(format(dataLocal[5], '#010b') + format(dataLocal[6], '08b'), 2))
#         pitchs.append(
#             int(format(dataLocal[7], '#010b') + format(dataLocal[8], '08b'), 2))
#         rolls.append(
#             int(format(dataLocal[9], '#010b') + format(dataLocal[10], '08b'), 2))
#         yaws.append(
#             int(format(dataLocal[11], '#010b') + format(dataLocal[12], '08b'), 2))
#         bars.append(
#             int(format(dataLocal[13], '#010b') + format(dataLocal[14], '08b'), 2))

#         # Counter for max messages
#         global count
#         count = count + 1

#         if (count >= maxMapMessages):
#             fingerprinting()
#             count = 0
#             clearVariables()

# Clear gateway data
def clearVariables():
    del mag_valueX[:]
    del mag_valueY[:]
    del mag_valueZ[:]
    del pitchs[:]
    del rolls[:]
    del yaws[:]
    del bars[:]

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
        print "Neighbour" + str(x) + ": " + distances[x][0][-1]
        neighbors.append(distances[x][0])
    return neighbors


# kNN algorithm - fingerprinting
previous_room = ""


def getResponse(neighbors):
    votes = {}
    for x in range(len(neighbors)):
        response = neighbors[x][-1].encode('ascii', 'ignore')
        if response in votes:
            votes[response][0] += 1
            votes[response][1].append(neighbors[x])
        else:
            votes[response] = [1, []]
            votes[response][1].append(neighbors[x])

    sortedVotes = sorted(
        votes.iteritems(), key=operator.itemgetter(1), reverse=True)
    return sortedVotes[0][0]


def fingerprinting():

    # Get TestData
    testInstance = []
    if len(mag_valueX) > 0:
        a = numpy.array(mag_valueX)
        testInstance.append(int(numpy.median(a)))
    else:
        testInstance.append(0)

    if len(mag_valueY) > 0:
        a = numpy.array(mag_valueY)
        testInstance.append(int(numpy.median(a)))
    else:
        testInstance.append(0)

    if len(mag_valueZ) > 0:
        a = numpy.array(mag_valueZ)
        testInstance.append(int(numpy.median(a)))
    else:
        testInstance.append(0)

    # if len(pitchs) > 0:
    #     a = numpy.array(pitchs)
    #     testInstance.append(int(numpy.median(a)))
    # else:
    #     testInstance.append(0)

    # if len(rolls) > 0:
    #     a = numpy.array(rolls)
    #     testInstance.append(int(numpy.median(a)))
    # else:
    #     testInstance.append(0)

    # if len(yaws) > 0:
    #     a = numpy.array(yaws)
    #     testInstance.append(int(numpy.median(a)))
    # else:
    #     testInstance.append(0)

    # if len(bars) > 0:
    #     a = numpy.array(bars)
    #     testInstance.append(int(numpy.median(a)))
    # else:
    #     testInstance.append(0)

    print "Testinstance: " + str(testInstance)
    neighbors = getNeighbors(trainSet, testInstance, k)
    current_room = getResponse(neighbors)

    global previous_room

    json_loc = {"node_id": "node1", "room_name": current_room}
    mqttc1.publish(MQTT_topic_RPi, str(json_loc))
    if(previous_room != current_room):
        print "Room: " + current_room
        print
        # json_loc = {"node_id": nodeid, "room_name": current_room}
        # mqttc2.connect(MQTT_RPi, 1883, 60)
        # mqttc2.publish(MQTT_topic_RPi, current_room)
        # mqttc2.disconnect()
        copyfile('/home/pi/localization_mag/images/' + current_room + '.png', '/etc/openhab2/html/localization_mag/current.png')
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


# MQTT - RPI INFO
print("Client: " + clientid)
print("Server: " + MQTT_RPi)
print("Topic: " + MQTT_topic_RPi)
print

# MQTT - SETUP METHODS
mqttc1.on_message = on_message

# MQTT - Create connection with server and subscribe/RPI publisch connect
mqttc1.connect(MQTT_RPi, 1883, 60)
mqttc1.subscribe(MQTT_topic_server)

# Get TrainingData
d = []
cursor = collection.find({})
for document in cursor:
    d.append(document)
trainSet = []
for fi in d:
    finger = []
    local = fi["room_name"] + "_box" + str(fi["fingerid"])
    RSSI_array = fi["RSSI"]
    for RSSI in RSSI_array[:-2]:
        finger.append(int(RSSI["RSSI-Value"]))
    finger.append(local)
    trainSet.append(finger)

count = 0
# Program loop
while True:
    mqttc1.loop()
