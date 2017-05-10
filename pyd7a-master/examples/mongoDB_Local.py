from sys import argv
import paho.mqtt.client as mqtt
import json
import math
import operator
from pymongo import MongoClient
# Willem --> "node": "b57000009128e"
# Frederik --> "node": "b57000009127b"
# Michiel --> "node": "b5700000913b8"
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
def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))
def on_message(mqttc, obj, msg):
    # Default incoming message
    # print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))
    # MESSAGE TO JSON
    payload = msg.payload
    payload_JSON = json.loads(payload)
    if (payload_JSON["node"] == nodeid):
        # print payload_JSON
        RSSI_value = payload_JSON["link_budget"]
        gatewayID = str(payload_JSON["gateway"])
        if gatewayID == 'b5700000912bf':
            if (len(gateway1) > 0):
                fingerprinting()
                del gateway1[:]
                del gateway2[:]
                del gateway3[:]
                del gateway4[:]
                del gateway5[:]
                del gateway6[:]
                gateway1.append(RSSI_value)
            else:
                gateway1.append(RSSI_value)
        if gatewayID == 'b5700000912fd':
            if (len(gateway2) > 0):
                fingerprinting()
                del gateway1[:]
                del gateway2[:]
                del gateway3[:]
                del gateway4[:]
                del gateway5[:]
                del gateway6[:]
                gateway2.append(RSSI_value)
            else:
                gateway2.append(RSSI_value)
        if gatewayID == 'b5700000912d9':
            if (len(gateway3) > 0):
                fingerprinting()
                del gateway1[:]
                del gateway2[:]
                del gateway3[:]
                del gateway4[:]
                del gateway5[:]
                del gateway6[:]
                gateway3.append(RSSI_value)
            else:
                gateway3.append(RSSI_value)
        if gatewayID == 'b570000091ac9':
            if (len(gateway4) > 0):
                fingerprinting()
                del gateway1[:]
                del gateway2[:]
                del gateway3[:]
                del gateway4[:]
                del gateway5[:]
                del gateway6[:]
                gateway4.append(RSSI_value)
            else:
                gateway4.append(RSSI_value)
        if gatewayID == 'b5700000912d5':
            if (len(gateway5) > 0):
                fingerprinting()
                del gateway1[:]
                del gateway2[:]
                del gateway3[:]
                del gateway4[:]
                del gateway5[:]
                del gateway6[:]
                gateway5.append(RSSI_value)
            else:
                gateway5.append(RSSI_value)
        if gatewayID == 'b570000091291':
            if (len(gateway6) > 0):
                fingerprinting()
                del gateway1[:]
                del gateway2[:]
                del gateway3[:]
                del gateway4[:]
                del gateway5[:]
                del gateway6[:]
                gateway6.append(RSSI_value)
            else:
                gateway6.append(RSSI_value)
def on_publish(mqttc, obj, mid):
    print("mid: " + str(mid))
def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))
def on_log(mqttc, obj, level, string):
    print(string)
def euclideanDistance(instance1, instance2, length):
    distance = 0
    for x in range(length):
        distance += pow((instance1[x] - instance2[x]), 2)
    return math.sqrt(distance)
def getNeighbors(trainingSet, testInstance, k):
    distances = []
    length = len(testInstance)-1
    for x in range(len(trainingSet)):
        dist = euclideanDistance(testInstance, trainingSet[x], length)
        distances.append((trainingSet[x], dist))
    distances.sort(key=operator.itemgetter(1))
    neighbors = []
    for x in range(k):
        neighbors.append(distances[x][0])
    return neighbors
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
        room_id = fi["room_id"]
        RSSI_array = fi["RSSI"]
        for RSSI in RSSI_array:
            finger.append(RSSI["RSSI-Value"])
        finger.append(room_name)
        finger.append(room_id)
        trainSet.append(finger)
    # trainSet = []
    # with open('V-blok.JSON') as json_data:
    #     d = json.load(json_data)
    #     for x in range(0, len(d)):
    #         trainSetTest = [int(d[x]['RSSI'][0]['RSSI-Value']), int(d[x]['RSSI'][1]['RSSI-Value']), int(d[x]['RSSI'][2]['RSSI-Value']),
    #                         int(d[x]['RSSI'][3]['RSSI-Value']), int(d[x]['RSSI'][4]['RSSI-Value']), int(d[x]['RSSI'][5]['RSSI-Value']),
    #                         d[x]['room_name'], d[x]['room_id']]
    #         trainSet.append(trainSetTest)
    # print "trainSet: " + str(trainSet)
    testInstance = []
    if len(gateway1) > 0:
        testInstance.append(int(gateway1[0]))
    else:
        testInstance.append(0)
    if len(gateway2) > 0:
        testInstance.append(int(gateway2[0]))
    else:
        testInstance.append(0)
    if len(gateway3) > 0:
        testInstance.append(int(gateway3[0]))
    else:
        testInstance.append(0)
    if len(gateway4) > 0:
        testInstance.append(int(gateway4[0]))
    else:
        testInstance.append(0)
    if len(gateway5) > 0:
        testInstance.append(int(gateway5[0]))
    else:
        testInstance.append(0)
    if len(gateway6) > 0:
        testInstance.append(int(gateway6[0]))
    else:
        testInstance.append(0)
    # print "testInstance: " + str(testInstance)
    k = 1
    neighbors = getNeighbors(trainSet, testInstance, k)
    print "You are in"
    print "Room: " + str(neighbors[0][6])
    #print "ID: " + str(neighbors[0][7])
    print
#Connect to MongoDB
client = MongoClient('localhost', 27017)
db = client['Ambient']
collection = db['V-blok']
# #Get TrainingData
# d = []
# cursor = collection.find({})
# for document in cursor:
#     d.append(document)
# CLIENT INFO
clientid = "Fredsons"
clientprotocol = 3  # MQTTv3.1
mqttc = mqtt.Client(client_id=clientid, protocol=clientprotocol)
nodeid = "b57000009128e"
# SETUP METHODS
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
# mqttc.on_log = on_log
# MQTT SERVER INFO
MQTT_server = "backend.idlab.uantwerpen.be"
MQTT_topic = "/localisation/DASH7"
print("Client: " + clientid)
print("Server: " + MQTT_server)
print("Topic: " + MQTT_topic)
print
print("Create connection....")
mqttc.connect(MQTT_server, 1883, 60)
mqttc.subscribe(MQTT_topic)
print("Connection created....\n")
while True:
    mqttc.loop()
