from picamera import PiCamera
from time import sleep
import os
import subprocess

camera = PiCamera()

camera.rotation = 0
camera.start_preview()

bashCommand='sudo convert cameratemp.jpg -pointsize 24 -fill #00CCFF -annotate +380+50 %[exif:DateTimeOriginal] cameratemp.jpg'

#camera.resolution = (640, 480)
while True:
  camera.capture('cameratemp.jpg')
  process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
  output, error = process.communicate()
  os.rename("cameratemp.jpg", "camera.jpg")
  sleep(4)

camera.stop_preview()
