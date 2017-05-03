from sys import argv
import paho.mqtt.client as mqtt
import json
prompt = '> '

def on_connect(mqttc, obj, flags, rc):
    print("rc: "+str(rc))

def on_message(mqttc, obj, msg):
    #Default incoming message
    #print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))

    #MESSAGE TO JSON
    payload = msg.payload
    payload_JSON = json.loads(payload)

    if(payload_JSON["node"]==nodeid):
        print payload_JSON
        f.write(str(payload_JSON))
        global count
        count = count + 1

def on_publish(mqttc, obj, mid):
    print("mid: "+str(mid))

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqttc, obj, level, string):
    print(string)

#CLIENT INFO
clientid = "Willem-develop"
clientprotocol = 3 #MQTTv3.1
mqttc = mqtt.Client(client_id=clientid, protocol=clientprotocol)
nodeid = "b57000009128e"

#SETUP METHODS
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
#mqttc.on_log = on_log

#MQTT SERVER INFO
MQTT_server = "backend.idlab.uantwerpen.be"
MQTT_topic = "/localisation/DASH7"

print("Client: " + clientid)
print("Server: " + MQTT_server)
print("Topic: " + MQTT_topic)
print

# CREATE JSON FILE
print "Creating new JSON file..."
print "Give location name:"
file_name = raw_input(prompt)
file_path = "../Mapping/"
f = open( file_path + file_name + ".JSON", 'a')

#Create connection
print("Create connection....")
mqttc.connect(MQTT_server, 1883, 60)
mqttc.subscribe(MQTT_topic)
print("Connection created....\n")


count = 0
while count < 20:
    mqttc.loop()

f.close()