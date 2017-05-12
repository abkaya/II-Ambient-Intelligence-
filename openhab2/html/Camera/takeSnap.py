from picamera import PiCamera
from time import sleep
import os

camera = PiCamera()

camera.rotation = 0
camera.start_preview()

#camera.resolution = (640, 480)
while True:  
	camera.capture('cameratemp.jpg')
	os.rename("cameratemp.jpg", "camera.jpg")
	sleep(4)
camera.stop_preview()
