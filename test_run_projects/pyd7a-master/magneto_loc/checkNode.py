#!/usr/bin/env python

import argparse

from d7a.alp.command import Command
from d7a.alp.interface import InterfaceType
from d7a.d7anp.addressee import Addressee, IdType
from d7a.sp.configuration import Configuration
from d7a.sp.qos import QoS, ResponseMode
from d7a.system_files.uid import UidFile
from modem.modem import Modem
import time

# Variables
mag_valueX = []
mag_valueY = []
mag_valueZ = []
pitchs = []
rolls = []
yaws = []
bars = []


def received_command_callback(cmd):
    dataLocal = cmd.actions[0].operand.data
    dataHeader = int(format(dataLocal[0], '#010b'),0)
    if(dataHeader == 42): # Only process data if first byte equals '42' (= data header for mobile node). This prevents conflicts with the NFC node
          print "MagX: " + str(int(format(dataLocal[1], '#010b') + format(dataLocal[2], '08b'),2))
          print "MagY: " + str(int(format(dataLocal[3], '#010b') + format(dataLocal[4], '08b'),2))
          print "MagZ: " + str(int(format(dataLocal[5], '#010b') + format(dataLocal[6], '08b'),2))
          print "Pitch: " + str(int(format(dataLocal[7], '#010b') + format(dataLocal[8], '08b'),2)/900)
          print "Roll: " + str(int(format(dataLocal[9], '#010b') + format(dataLocal[10], '08b'), 2)/900)
          print "Yaw: " + str(int(format(dataLocal[11], '#010b') + format(dataLocal[12], '08b'), 2)/900)
          print 

argparser = argparse.ArgumentParser()
argparser.add_argument("-d", "--device", help="serial device /dev file modem",
                            default="/dev/ttyUSB0")
argparser.add_argument("-r", "--rate", help="baudrate for serial device", type=int, default=115200)
argparser.add_argument("-v", "--verbose", help="verbose", default=False, action="store_true")
config = argparser.parse_args()

modem = Modem(config.device, config.rate, receive_callback=received_command_callback, show_logging=config.verbose)

modem.start_reading()


while True:
  pass
