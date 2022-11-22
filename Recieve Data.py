import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

plt.style.use("seaborn-v0_8-dark")
ser = serial.Serial("COM5", 9600, timeout = 1) #Change your port name COM... and your baudrate
print("*****CONECTED*****")

x = []
sensor1 = []
sensor2 = []
mva1 = []
mva2 = []
prev = time.time();
def animate(i):
    global prev
    data = retrieveData()
    if(data != ''):
        dataStr = data.split(',')
        vals = []
        for j in range(len(dataStr)):   
            vals.append(float(dataStr[j]))
            if vals[j] > 20.0:
                vals[j] = 20.0

        sensor1.append(vals[0])
        sensor2.append(vals[1])
        x.append(time.time()-prev)

        #Moving Average
        if(len(x) > 10):
            mva1.append(sum(sensor1[-10:])/10)
            mva2.append(sum(sensor2[-10:])/10)
        else:
            mva1.append(0)
            mva2.append(0)
            
        if(len(x) > 50):
            x.pop(0)
            sensor1.pop(0)
            sensor2.pop(0)
            mva1.pop(0)
            mva2.pop(0)
        
        plt.cla()  # clears the values of the graph
        plt.plot(x, sensor1, linestyle='solid', label ='Right Sensor 1')
        plt.plot(x, mva1, linestyle='dotted', label ='MVA Right Sensor 1')

        plt.plot(x, sensor2, linestyle='solid', label ='Right Sensor 2')
        plt.plot(x, mva2, linestyle='dotted', label ='MVA Right Sensor 2')

        plt.legend(loc='upper right')
        plt.xlabel('Time (s)')

        plt.ylabel('Distance (cm)')
        plt.ylim([0, 20])

        plt.title('Sensor Data')
        plt.tight_layout()


def retrieveData():
    data = ser.readline().decode('ascii')
    return data


ani = FuncAnimation(plt.gcf(), animate, 5)
plt.tight_layout()
plt.show()

    
