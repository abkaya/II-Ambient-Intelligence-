import datetime
import json
import os
import sys
from sys import argv

import numpy
import paho.mqtt.client as mqtt
from pymongo import MongoClient

import argparse

from d7a.alp.command import Command
from d7a.alp.interface import InterfaceType
from d7a.d7anp.addressee import Addressee, IdType
from d7a.sp.configuration import Configuration
from d7a.sp.qos import QoS, ResponseMode
from d7a.system_files.uid import UidFile
from modem.modem import Modem
import time

prompt = '> '

# Variables
mag_values = []
pitchs = []
rolls = []
yaws = []
bars = []

#Accuracy
maxMapMessages = 2
MaxAccuracy = 1

#Connections
group_name = "Team-MetVier"
collection_name = "V-Blok_mag"

def received_command_callback(cmd):
    if(check):
        dataLocal = cmd.actions[0].operand.data
        print dataLocal
        mag_values.append(int(format(dataLocal[0], '#010b') + format(dataLocal[1], '08b'),2))
        pitchs.append(int(format(dataLocal[2], '#010b') + format(dataLocal[3], '08b'), 2))
        rolls.append(int(format(dataLocal[4], '#010b') + format(dataLocal[5], '08b'), 2))
        yaws.append(int(format(dataLocal[6], '#010b') + format(dataLocal[7], '08b'), 2))
        #bars.append(int(format(dataLocal[8], '#010b') + format(dataLocal[9], '08b'), 2))

        # Counter for max messages
        global count
        count = count + 1

check = False
def mapData(room_name,boxid,fingerid):
    #Enter before mapping
    json_data = []

    #Read MQTT messages
    while count < maxMapMessages:
        global check
        check = True

    check = False

    #Take the median of each gateway    
    if len(mag_values) > 0:
        a = numpy.array(mag_values)
        RSSI_mag_value = numpy.median(a)
    if len(pitchs) > 0:
        a = numpy.array(pitchs)
        RSSI_pitch = numpy.median(a)
    if len(rolls) > 0:
        a = numpy.array(rolls)
        RSSI_roll = numpy.median(a)
    if len(yaws) > 0:
        a = numpy.array(yaws)
        RSSI_yaw = numpy.median(a)
    if len(bars) > 0:
        a = numpy.array(bars)
        RSSI_bar = numpy.median(a)

    #Create the mapping JSON
    data = {}
    data['room_name'] = room_name
    data['boxid'] = int(boxid)
    data['fingerid'] = fingerid
    data['data_type'] = "raw_mean"
    timestamp = datetime.datetime.now()
    data['timestamp'] = timestamp
    RSSI = []
    if len(mag_values) > 0:
        RSSI.append({"Variable": "mag_value", "RSSI-Value": int(RSSI_mag_value)})
    else:
        RSSI.append({"Variable": "mag_value", "RSSI-Value": 0})
    if len(pitchs) > 0:
        RSSI.append({"Variable": "pitch", "RSSI-Value": int(RSSI_pitch)})
    else:
        RSSI.append({"Variable": "pitch", "RSSI-Value": 0})
    if len(rolls) > 0:
        RSSI.append({"Variable": "roll", "RSSI-Value": int(RSSI_roll)})
    else:
        RSSI.append({"Variable": "roll", "RSSI-Value": 0})
    if len(yaws) > 0:
        RSSI.append({"Variable": "yaw", "RSSI-Value": int(RSSI_yaw)})
    else:
        RSSI.append({"Gateway": "yaw", "RSSI-Value": 0})
    if len(bars) > 0:
        RSSI.append({"Gateway": "bar", "RSSI-Value": int(RSSI_bar)})
    else:
        RSSI.append({"Gateway": "bar", "RSSI-Value": 0})

    data['RSSI'] = RSSI
    collection.insert(data)

#Connect to MongoDB
client = MongoClient('localhost', 27017)
db = client[group_name]
collection = db[collection_name]

# CREATE ROOM
print "Give room name:"
room_name = raw_input(prompt)
print "Give box id:"
boxid = raw_input(prompt)
fingerid = 0
count = 0

argparser = argparse.ArgumentParser()
argparser.add_argument("-d", "--device", help="serial device /dev file modem",
                            default="/dev/ttyUSB0")
argparser.add_argument("-r", "--rate", help="baudrate for serial device", type=int, default=115200)
argparser.add_argument("-v", "--verbose", help="verbose", default=False, action="store_true")
config = argparser.parse_args()

modem = Modem(config.device, config.rate, receive_callback=received_command_callback, show_logging=config.verbose)

modem.start_reading()

while True:
    raw_input(prompt)
    for x in range(MaxAccuracy):
        mapData(room_name, boxid, fingerid)
        count = 0
    print "Room: " + room_name
    print "Box: " + str(boxid) + "_" + str(fingerid)
    fingerid+=1
