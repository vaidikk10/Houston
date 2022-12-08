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
pinMode(10,OUTPUT);      //LF
pinMode(11,INPUT);
pinMode(A0,OUTPUT);      //LB
pinMode(A1,INPUT);

  
  HC06.begin(9600); //Baudrate 9600
  Serial.begin(9600);
  prev = millis();   // Time (ms) since arduino started
//  SPEED = 100;
  
  Serial.println("SERIAL HAS BEGUN\n"); // print some text in Serial Monitor
  robot = new Robot();
}


void loop() 
{
  robot->readSensors();
  switch (robot->STATE)
  {
    case Robot::START:   //  *** OUTSIDE OF MAZE ***
      robot->straight();
      if (robot->hasEnteredMaze())
      robot->STATE = Robot::SEARCHING;
      break;
    
    case Robot::SEARCHING:
       robot->straight();
//       robot->makeCentre();
//       delay(20);
       robot->makeParallel();
       delay(25);
       robot->straight();
       if (robot->SensorFront->getReading() < 10) 
       {
      if (robot->isCorner())
      { robot->STATE = Robot::AT_CORNER; }
      else if (robot->isTJunction())
      { robot->STATE = Robot::AT_TJUNCTION; }
      else if (robot->isDeadEnd())
      { robot->STATE = Robot::AT_DEADEND; }
       }
      break;
       
    case Robot::STOP:
      robot->stopBot();
      robot->straight();
      break;
      
    case Robot::AT_CORNER:
      if (robot->CORNER_DIRECTION == Robot::LEFT)
      { 
        robot->turnLeft();
        delay(1000);
        robot->STATE = Robot::SEARCHING;
        break;
        while (1)
        {
          if ( robot->turnLeft() )
          {
            robot->STATE = Robot::SEARCHING;
            robot->straight();
            break;
          }
        }
      }
      else if (robot->CORNER_DIRECTION == Robot::RIGHT)
      {
        robot->turnRight();
        delay(1000);
        robot->STATE = Robot::SEARCHING;
        break;
        while (1)
        {
          if ( robot->turnRight() )
          {
            robot->STATE = Robot::SEARCHING;
            robot->straight();
            break;
          }
        }
      }
      break;
      
    case Robot::AT_TJUNCTION:
      robot->stopBot();
      break;
      
    case Robot::AT_DEADEND:
      robot->stopBot();
      break;
      
    case Robot::REVERSING:      // maybe count turns after wrong turn until deaedend, so we know when were back to T Junction
      break;
      
    default:
      break;
  } 
}

double getDistance(int trigPin, int echoPin){
  double duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  if(duration ==0){
    Serial.print("Sensor Disconnected [Trig, Echo] : ");
    Serial.print(trigPin);
    Serial.print(", ");
    Serial.println(echoPin);
    return -1;
  }else{
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  }
  
  delay(500);
  return distance;
}















// ---------------------------- TEST WITH NEW SENSOR READ ROBOT METHOD ----------------------------
