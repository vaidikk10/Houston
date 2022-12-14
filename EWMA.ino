//Front Sensor Wall Detection and Movemnt
//Sends sensor data to python using HC-06

// Red wires are Vcc, Brown wires are ground
// lighter colours are trig, darker colours are echo

#include <Servo.h>
#include <SoftwareSerial.h>
// #include "RunningAverage.h"          // https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage
#include "Robot.h"




SoftwareSerial HC06(12, 13); //HC06-TX Pin 10, HC06-RX to Arduino Pin 11
// RunningAverage MVA(100);

//unsigned long prev;

//enum direct {LEFT, RIGHT} rightOrLeft;

Robot *robot;


void setup() {


pinMode(2,OUTPUT);      //RF
pinMode(3,INPUT);    
pinMode(4,OUTPUT);      //RB
pinMode(5,INPUT);
pinMode(6,OUTPUT);      //F
pinMode(7,INPUT);
pinMode(10,OUTPUT);      //LF
pinMode(11,INPUT);
pinMode(A0,OUTPUT);      //LB
pinMode(A1,INPUT);

  
  HC06.begin(9600); //Baudrate 9600
  Serial.begin(9600);
//  prev = millis();   // Time (ms) since arduino started
  
  Serial.println("SERIAL HAS BEGUN\n"); // print some text in Serial Monitor
  robot = new Robot();    // call constructor on robot
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
       robot->makeCentre();
       robot->makeParallel();

       if (robot->SensorFront->getReading() < 10) // START CHECKING FOR CORNERS WHEN WITHIN 10CM OF WALL  
       {
        if (robot->isCorner() )
        { robot->STATE = Robot::AT_CORNER; }
        else if (robot->isTJunction())
        { robot->STATE = Robot::AT_TJUNCTION; }
        else if (robot->isDeadEnd())
        { robot->STATE = Robot::AT_DEADEND; }
       }
      break;
       
    case Robot::STOP:
      robot->stopBot();
      break;
      
    case Robot::AT_CORNER:
      if (robot->CORNER_DIRECTION == Robot::LEFT)
      { 
        robot->turnLeft();
        delay(100);
        while (1)
        {
          if (robot->isParallel(Robot::LEFT) && robot->isParallel(Robot::LEFT)) break;     // 2 in a row needed.
        }
        robot->STATE = Robot::SEARCHING;
        robot->straight();
      }
      else if (robot->CORNER_DIRECTION == Robot::RIGHT)
      {
        robot->turnRight();
        delay(100);
        while (1)
        {
          if (robot->isParallel(Robot::RIGHT) && robot->isParallel(Robot::RIGHT)) break;
        }
        robot->STATE = Robot::SEARCHING;
        robot->straight();
      }
      break;
      
    case Robot::AT_TJUNCTION:
      if ( robot->Runs.currentRun == 0 )
      {
        robot->CORNER_DIRECTION = Robot::RIGHT;
      }else if ( robot->Runs.currentRun == 1 ) 
      {
        robot->CORNER_DIRECTION = Robot::LEFT;   
      } else if ( robot->Runs.currentRun == 2 )
      {
        if ( robot->Runs.fastestRun == 0 ) {robot->CORNER_DIRECTION = Robot::RIGHT;}
        else {robot->CORNER_DIRECTION = Robot::LEFT;}
      }
      robot->STATE = Robot::AT_CORNER;
      break;
      
    case Robot::AT_DEADEND:
      if ( robot->Runs.currentRun == 0 ) {robot->Runs.fastestRun = 1;} // if deadend is reached in first run, second run (left) is correct way
      robot->turnRight();
      delay(2250);
      robot->STATE = Robot::SEARCHING;
      break;
      
    case Robot::REVERSING:      // maybe count turns after wrong turn until deaedend, so we know when were back to T Junction
      break;
      
    default:
      break;
  } 
}
  


// ---------------------------- TEST WITH NEW SENSOR READ ROBOT METHOD ----------------------------
