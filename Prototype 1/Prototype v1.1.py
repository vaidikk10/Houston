import serial
import json
import tkinter as tk
from tkinter import Button
import logging
import threading

STATES = ["START", "SEARCHING", "STOP", "AT_CORNER", "AT_TJUNCTION", "AT_DEADEND", "REVERSING"];
PORT = "COM5"

class MainWindow(tk.Tk):
    def __init__(self):
        super().__init__()
        self.t1Stop = False;
        self.initUI()
        
    def initUI(self):
        self.title("Houston GUI")
        self.geometry("500x500")
        
        self.connectionStatus = tk.Label(self, text="Connection Status: Not Connected")
        self.connectionStatus.pack()

        self.btnStart = Button(self, text="Start", command=self.startThread)
        self.btnStart.pack()
        
        self.RobotState = tk.Label(self, text="Robot State: N/A")
        self.RobotState.pack()
        
        self.RobotSpeed = tk.Label(self, text="Robot Speed: N/A")
        self.RobotSpeed.pack()
        
        self.RobotDirection = tk.Label(self, text="Robot Direction: N/A")
        self.RobotDirection.pack()
        
        self.Deadend = tk.Label(self, text="Deadend: N/A")
        self.Deadend.pack()
        
        self.InsideMaze = tk.Label(self, text="InsideMaze: N/A")
        self.InsideMaze.pack()

        self.finished = tk.Label(self, text="Finished: N/A")
        self.finished.pack()

        self.Started = tk.Label(self, text="Run: N/A")
        self.Started.pack()

        exitButton = Button(self, text="Exit", command=self.clickExitButton)
        exitButton.pack()
        

    def clickExitButton(self):
        self.t1Stop = True
        exit()
        logging.error("Exiting program")

    def startThread(self):
        self.retrieveData();
        self.t1 = threading.Thread(target=self.updateWindow)
        self.t1.start()
        self.btnStart.config(state="disabled")
    
    def updateWindow(self):
        while True:
            data = self.retrieveData()
            if(data != ''):
                data = data.rsplit(',',1)[0] + "}" #remove the last ',' and add a '}'
                json_data = json.loads(data.replace("'", "\""))
                # print(STATES[int(json_data["State"])])

                #Update all the variables
                self.RobotState.config(text="Robot State: " + STATES[int(json_data["State"])])
                if self.t1Stop:
                    return

    def retrieveData(self):
        try:
            data = self.ser.readline().decode('ascii')
            logging.info("Data received: " + data)
        except Exception as e:
            logging.error("Data receive failed: " + str(e))
            data = ''
        return data

    def connect(self):
        logging.info("Connecting to HC-06")
        self.connectionStatus.config(text="Connection Status: Connecting...")
        try:
            self.ser = serial.Serial(PORT, 9600, timeout = 1) #Change your port name COM... and your baudrate
            self.connectionStatus.config(text="Connection Status: Connected")
            logging.info("Connected Sucessfull to HC-06")
            self.retrieveData()
        except Exception as e:
            self.connectionStatus.config(text="Connection Status: Not Connected")
            logging.error("Connection Failed to HC-06: " + str(e))


if __name__ == '__main__':
    window = MainWindow()
    window.connect()
    window.mainloop()
    window.t1.join()
    
    