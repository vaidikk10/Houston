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
    // ********** Pin Setup ***********
//  for (int i = 2; i <= 13; i++)
//  {
//    if (i % 2)
//    {
//      pinMode(i, OUTPUT);
//      continue;
//    } 
//    pinMode(i, INPUT);
//  }
pinMode(6,OUTPUT);
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
// Serial.print("Front Sensor Reading: ");
// Serial. println(getReading(6,7));
// Serial.print("robot.STATE: ");
// Serial.println(robot->STATE);
 
  switch (robot->STATE)
  {
    case Robot::START:   //  *** OUTSIDE OF MAZE 
      Serial.print("Distance recieved: ");
      Serial.println(robot->SensorFront->getReading2());
      Serial.print("Distance striaght from object: ");
      Serial.println(robot->SensorFront->sonar->ping_cm());
  
//     robot->straight();
//    if (!robot.hasEnteredMaze())
    {
      break;
    }
  case Robot::SEARCHING:
    if (!robot->isParallel()) robot->makeParallel();
    robot->straight();
    break;
  case Robot::STOP:
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


//
//void straight(){
//  ServoLeft.attach(13);
//  ServoRight.attach(12);
//  ServoLeft.writeMicroseconds(2000 - SPEED);
//  ServoRight.writeMicroseconds(1000 + SPEED);
//}
//
//void right(){
//  ServoLeft.attach(12);
//  ServoRight.attach(13);
//  ServoLeft.writeMicroseconds(1500 - SPEED);
//  ServoRight.writeMicroseconds(1500);
//}
//
//void left(){
//  ServoLeft.attach(13);
//  ServoRight.attach(12);
//  ServoLeft.writeMicroseconds(1500);
//  ServoRight.writeMicroseconds(1500 + SPEED);
//}
//
//void stopBot(){
//  //ServoLeft.writeMicroseconds(1500);
//  //ServoRight.writeMicroseconds(1500);
//    ServoLeft.detach();
//  ServoRight.detach();
//}
//
//
//void turnCorner(int direc){
//  if (direc == LEFT)
//  {
//    left();
//  }
//  else if (direc == RIGHT)
//  {
//    right();
//  }
//}
