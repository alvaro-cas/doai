import serial
import time
import calendar

while True:
    time.sleep(1)
    gmt = time.gmtime()
    data = calendar.timegm(gmt)

    print(data)

    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.write(bytes(str(data),"utf-8"))
    ser.close()


