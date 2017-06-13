#!/usr/bin/env python

import argparse

from d7a.alp.command import Command
from d7a.alp.interface import InterfaceType
from d7a.d7anp.addressee import Addressee, IdType
from d7a.sp.configuration import Configuration
from d7a.sp.qos import QoS, ResponseMode
from d7a.system_files.uid import UidFile
from modem.modem import Modem
import paho.mqtt.publish as publish
import datetime


def received_command_callback(cmd):
    # print data
    # print cmd.actions[0].operand.data
    data = cmd.actions[0].operand.data
    data = map(int, data)
    print(data)
    if data[0]==199:
        publish_magnetometer(data)
    if (data[0] == 0 and data[1] == 19):
        publish_altitude(data)
    if (data[0] == 42 and data[1] == 42):
        publish_localization(data)
    if (data[0] == 4 and data[1] == 115 and data[3] == 187):
        publish_nfc(data)
    # mockup switch case, using dictionaries
    # options[data[1]](data)


def publish_magnetometer(data):
    x = (data[2] << 8) | data[3]
    y = (data[4] << 8) | data[5]
    z = (data[6] << 8) | data[7]
    # x is a two's complement integer with two bytes, therefore it needs the proper conversion for its signedness when negative
    if is_twos_negative(x):
        x = x - (1 << 16)
    if is_twos_negative(y):
        y = y - (1 << 16)
    if is_twos_negative(z):
        z = z - (1 << 16)
    publish.single("sensordata/magnetometerX", str(x), hostname="localhost")
    publish.single("sensordata/magnetometerY", str(y), hostname="localhost")
    publish.single("sensordata/magnetometerZ", str(z), hostname="localhost")


# publish altitude in meters
# 3rd floor : within range [28;34]
def publish_altitude(data):
    publish.single("mobilenode/altitude",data[3],hostname="localhost")

def publish_localization(data):
    publish.single("d7node/localization", data, hostname="localhost")

def publish_nfc(data):
    publish.single("nfcnode/tag", ('Abdil at {: %H:%M:%S %Y-%b-%d}'.format(datetime.datetime.now())), hostname="localhost")


def is_twos_negative(val):
    constant = 1 << (15);
    if (val & constant):
        return 1;
    return 0


# map the inputs to the function blocks
options = {199: publish_magnetometer,
           }

argparser = argparse.ArgumentParser()
argparser.add_argument("-d", "--device", help="serial device /dev file modem",
                       default="/dev/ttyACM1")
argparser.add_argument("-r", "--rate", help="baudrate for serial device", type=int, default=115200)
argparser.add_argument("-v", "--verbose", help="verbose", default=False, action="store_true")
config = argparser.parse_args()

modem = Modem(config.device, config.rate, receive_callback=received_command_callback, show_logging=config.verbose)

modem.d7asp_fifo_flush(
    alp_command=Command.create_with_read_file_action_system_file(
        file=UidFile(),
        interface_type=InterfaceType.D7ASP,
        interface_configuration=Configuration(
            qos=QoS(resp_mod=ResponseMode.RESP_MODE_ALL),
            addressee=Addressee(
                access_class=0x01,
                id_type=IdType.NOID
            )
        )
    )
)

modem.start_reading()
while True:
    pass
