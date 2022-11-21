import serial

ser = serial.Serial("COM6", 9600, timeout = 1) #Change your port name COM... and your baudrate

def retrieveData():
    data = ser.readline().decode('ascii')
    print(data)

while(True):
    retrieveData()