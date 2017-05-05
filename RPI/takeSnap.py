from picamera import PiCamera
from time import sleep
import os

camera = PiCamera()

camera.rotation = 0 
camera.start_preview()

# use os.rename in order to prevent temporary black screens in openhab while 
# the camera is taking a picture and there is no valid .jpf format to present in the panel
while True:  
        camera.capture('cameratemp.jpg')
        os.rename("cameratemp.jpg", "camera.jpg")
        sleep(1)
camera.stop_preview()
