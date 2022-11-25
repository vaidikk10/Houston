//Front Sensor Wall Detection and Movemnt
//Sends sensor data to python using HC-06

#include <Servo.h>
#include <SoftwareSerial.h>
#include "RunningAverage.h"          // https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage

SoftwareSerial HC06(10, 11); //HC06-TX Pin 10, HC06-RX to Arduino Pin 11
RunningAverage MVA(200);

Servo ServoLeft;      // 1000 - 1499 to go forward
Servo ServoRight;     // 1501 - 2000 to go forward

unsigned long prev;
int count = 0;
unsigned short SPEED;
float avg = 0;
boolean STOP = false; //False means keep going

void setup() {
  prev = millis();
  SPEED = 100;
  //Right Sensors
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);

  HC06.begin(9600); //Baudrate 9600 , Choose your own baudrate
  Serial.begin(9600);
  ServoLeft.attach(12);
  ServoRight.attach(13);

  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
  getDistance(8,9);
}

void loop() {
  double frontSensor =  getDistance(8, 9);
//    avg = frontSensor;
    avg = MVA.getAverage();
    if(avg <= 6 || frontSensor <= 4)
    {
      Serial.println("Stop");
      stopBot();
      STOP = true;
    }else{
      Serial.println("Keep Going");
      straight();
      STOP = false;
    }
    if(millis()-prev> 500){
           Serial.print(frontSensor);
      Serial.print(",");
      Serial.println(frontSensor);
    HC06.println(avg);
    prev = millis();
    }
  }

double getDistance(int trigPin, int echoPin) {
  double duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  if (duration == 0) {
    Serial.print("Sensor Disconnected [Trig, Echo] : ");
    Serial.print(trigPin);
    Serial.print(", ");
    Serial.println(echoPin);
    return -1;
  } else {
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    if(distance >=150){
      distance = 150;
    }
    MVA.add(distance);
  }

  delay(10);
  return distance;
}

void straight(){
    ServoLeft.attach(12);
  ServoRight.attach(13);
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

void right(){
  ServoLeft.writeMicroseconds(1500 - SPEED);
  ServoRight.writeMicroseconds(1500);
}

void left(){
  ServoLeft.writeMicroseconds(1500);
  ServoRight.writeMicroseconds(1500 + SPEED);
}

void stopBot(){
  //ServoLeft.writeMicroseconds(1500);
  //ServoRight.writeMicroseconds(1500);
    ServoLeft.detach();
  ServoRight.detach();
}
