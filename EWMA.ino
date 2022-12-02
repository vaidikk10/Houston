//Front Sensor Wall Detection and Movemnt
//Sends sensor data to python using HC-06

// Red wires are Vcc, Brown wires are ground
// lighter colours are trig, darker colours are echo

#include <Servo.h>
#include <SoftwareSerial.h>
// #include "RunningAverage.h"          // https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage
#include "Robot.h"




SoftwareSerial HC06(12, 13); //HC06-TX Pin 10, HC06-RX to Arduino Pin 11
//RunningAverage MVA(100);

unsigned long prev;
int count = 0;
// unsigned short SPEED;
float avg = 0;
boolean STOP = false; //False means keep going

//enum direct {LEFT, RIGHT} rightOrLeft;

Robot *robot;


void setup() {


pinMode(2,OUTPUT); //RF
pinMode(3,INPUT);    
pinMode(4,OUTPUT);   //RB
pinMode(5,INPUT);
pinMode(6,OUTPUT);      //F
pinMode(7,INPUT);
  
  HC06.begin(9600); //Baudrate 9600
  Serial.begin(9600);
  prev = millis();   // Time (ms) since arduino started
//  SPEED = 100;
  
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
  robot = new Robot();
}


void loop() 
{
  switch (robot->STATE)
  {
    case Robot::START:   //  *** OUTSIDE OF MAZE 
      robot->straight();
      robot->STATE = Robot::SEARCHING;
      break;
    
  case Robot::SEARCHING:
//    if (!robot->isParallel()) robot->makeParallel();
    robot->makeParallel();
    robot->straight();
    break;
  case Robot::STOP:
    robot->stopBot();
    break;
  case Robot::AT_CORNER:
    break;
  case Robot::AT_TJUNCTION:
    break;
  case Robot::AT_DEADEND:
    break;
  case Robot::REVERSING:
    break;
  default:
    break;
  } 
}







double getReading(int trigPin, int echoPin)
{
  double duration, distance;
  digitalWrite(trigPin, LOW);   // reset to low position then pulse on to trigger
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  Serial.println(duration);
  if (duration == 0) {
    Serial.print("Sensor Disconnected [Trig, Echo] : ");
    Serial.print(trigPin);
    Serial.print(", ");
    Serial.println(echoPin);
    // MAYBE PUT AN AVERAGE VALUE IN HERE FOR THE PAST ELEMENT WMA ARRAY???
    return -1;  
  } else {
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    if(distance > 150){
      distance = 150;
    }
    return distance;
  }
}
