import serial
import time

serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

print("init")
while True:
    msg = s.read(12)
    print(msg.decode())
s.close()