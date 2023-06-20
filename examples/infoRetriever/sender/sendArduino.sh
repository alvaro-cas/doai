#!/usr/bin/python

import sys
import serial

data = sys.argv[1:]
data = ','.join(data)
print(data)

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.write(bytes(data,"utf-8"))
ser.close()
