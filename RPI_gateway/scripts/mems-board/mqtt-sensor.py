import paho.mqtt.publish as publish
import time

i = 0
while True:
	
	publish.single("sensordata/testgraph", str(i), hostname="localhost")
	i = i + 1
	time.sleep(10)
	if i == 100:
		i = 0
