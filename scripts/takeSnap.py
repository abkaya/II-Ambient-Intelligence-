from picamera import PiCamera
from time import sleep
import os
import subprocess

camera = PiCamera()

camera.rotation = 0
camera.start_preview()

bashCommand="sudo convert cameratemp.jpg -pointsize 24 -fill yellow -annotate +270+460 %[exif:DateTimeOriginal] cameratemp1.jpg"

#camera.resolution = (640, 480)
while True:
  camera.capture('cameratemp.jpg')
  process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
  output, error = process.communicate()
  os.rename("cameratemp1.jpg", "camera.jpg")
  sleep(4)

camera.stop_preview()
