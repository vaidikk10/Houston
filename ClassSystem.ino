//Front Sensor Wall Detection and Movemnt
//Sends sensor data to python using HC-06


//***************************** SENSOR PINS *****************************
#define RIGHT_FRONT_TRIG 2
#define RIGHT_FRONT_ECHO 3
#define RIGHT_BACK_TRIG 4
#define RIGHT_BACK_ECHO 5
#define FRONT_TRIG 6
#define FRONT_ECHO 7
#define BACK_TRIG 8
#define BACK_ECHO 9
#define LEFT_FRONT_TRIG 10
#define LEFT_FRONT_ECHO 11
#define LEFT_BACK_TRIG 12
#define LEFT_BACK_ECHO 13

// Red wires are Vcc, Brown wires are ground
// lighter colours are trig, darker colours are echo




#include <Servo.h>
#include <SoftwareSerial.h>
#include "RunningAverage.h"          // https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage

SoftwareSerial HC06(12, 13); //HC06-TX Pin 10, HC06-RX to Arduino Pin 11
RunningAverage MVA(100);

Servo ServoLeft;      // 1000 - 1499 to go forward
Servo ServoRight;     // 1501 - 2000 to go forward



unsigned long prev;
int count = 0;
unsigned short SPEED;
float avg = 0;
boolean STOP = false; //False means keep going

enum direct {LEFT, RIGHT} rightOrLeft;

void setup() {
  prev = millis();
  SPEED = 100;

  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);

  HC06.begin(9600); //Baudrate 9600 , Choose your own baudrate
  Serial.begin(9600);
  ServoLeft.attach(12);
  ServoRight.attach(13);

  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");

}

void loop() {
  double frontSensor =  getDistance(FRONT_TRIG, FRONT_ECHO);
  double leftSensor = getDistance(10, 11);
  double rightSensor = getDistance(2, 3);
//    avg = frontSensor;
    MVA.add(frontSensor);
//    avg = MVA.getAverage();
    if(MVA.getAverage()<= 7 || frontSensor <= 5)   // avg was 6 before
    {
      Serial.println("Stop");
      stopBot();
      STOP = true;
      if (leftSensor > 15)
      {
        rightOrLeft = LEFT;
      }else if (rightSensor > 15)
      {
        rightOrLeft = RIGHT;
      }
      //turnCorner(rightOrLeft);
    }else{
//      Serial.println("Keep Going");
      straight();
      STOP = false;
    }
    if(millis()-prev> 500){
      Serial.print(frontSensor);
      Serial.print(",");
      Serial.println(frontSensor);
    HC06.println(frontSensor);
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
  }

  delay(10);
  return distance;
}

void straight(){
  ServoLeft.attach(13);
  ServoRight.attach(12);
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

void right(){
  ServoLeft.attach(12);
  ServoRight.attach(13);
  ServoLeft.writeMicroseconds(1500 - SPEED);
  ServoRight.writeMicroseconds(1500);
}

void left(){
  ServoLeft.attach(13);
  ServoRight.attach(12);
  ServoLeft.writeMicroseconds(1500);
  ServoRight.writeMicroseconds(1500 + SPEED);
}

void stopBot(){
  //ServoLeft.writeMicroseconds(1500);
  //ServoRight.writeMicroseconds(1500);
    ServoLeft.detach();
  ServoRight.detach();
}


void turnCorner(int direc){
  if (direc == LEFT)
  {
    left();
  }
  else if (direc == RIGHT)
  {
    right();
  }
}
