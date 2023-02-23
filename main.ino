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

Robot robot;


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
  
  Serial.println("SERIAL HAS BEGUN\n"); // print some text in Serial Monitor
//  attachInterrupt(digitalPinToInterrupt(2), ButtonPressed_EXTI0, FALLING);      // Declare ButtonPressed_EXTI0 as ISR on pin 2
}



void loop() 
{
  switch (robot.STATE)
  {
    case Robot::START:   //  *** OUTSIDE OF MAZE ***
      robot.readSensors();
      robot.straight();
      if (robot.hasEnteredMaze())
      robot.STATE = Robot::SEARCHING;
      break;
    
    case Robot::SEARCHING:
       robot.readSensors();
       robot.straight();
       robot.makeCentre();
       robot.makeParallel();

       if (robot.SensorFront->getReading() < 10) // START CHECKING FOR CORNERS WHEN WITHIN 10CM OF WALL  
       {
        if (robot.isCorner() )
        { robot.STATE = Robot::AT_CORNER; }
        else if (robot.isTJunction())
        { robot.STATE = Robot::AT_TJUNCTION; }
        else if (robot.isDeadEnd())
        { robot.STATE = Robot::AT_DEADEND; }
       }
      break;
       
    case Robot::STOP:
      robot.stopBot();
      break;
      
    case Robot::AT_CORNER:
      robot.readSensors();
      if (robot.CORNER_DIRECTION == Robot::LEFT)
      { 
        robot.turnLeft(400);
        delay(100);
        robot.STATE = Robot::TURNING_LEFT;
      }else if (robot.CORNER_DIRECTION == Robot::RIGHT)
      {
        robot.turnRight(400);
        delay(100);
        robot.STATE = Robot::TURNING_RIGHT;
      }
      break;

    case Robot::TURNING_LEFT:
        if ( robot.SensorRightFront->getReading() < robot.SensorRightBack->getReading() )
        {
          // reduce speed of turn
          robot.turnLeft(475);
        }else
        {
          //turn faster
          robot.turnLeft(400);
        }
        if (robot.isParallel(Robot::LEFT) && robot.isParallel(Robot::LEFT) && robot.isParallel(Robot::LEFT))     // 3 in a row needed. (end infinite loop when parallel)
        {
          robot.STATE = Robot::SEARCHING;
          robot.straight();
        }
    break;

    case Robot::TURNING_RIGHT:
        if ( robot.SensorLeftFront->getReading() < robot.SensorLeftBack->getReading() )
        {
          // reduce speed of turn
          robot.turnRight(475);
        }else
        {
          //turn faster
          robot.turnRight(400);
        }
        if (robot.isParallel(Robot::RIGHT) && robot.isParallel(Robot::RIGHT) && robot.isParallel(Robot::RIGHT))
        {
          robot.STATE = Robot::SEARCHING;
          robot.straight();
        }
    break;
      
    case Robot::AT_TJUNCTION:
      robot.readSensors();
      if ( robot.Runs.currentRun == 0 )
      {
        robot.CORNER_DIRECTION = Robot::RIGHT;
      }else if ( robot.Runs.currentRun == 1 ) 
      {
        robot.CORNER_DIRECTION = Robot::LEFT;   
      } else if ( robot.Runs.currentRun == 2 )
      {
        if ( robot.Runs.fastestRun == 0 ) {robot.CORNER_DIRECTION = Robot::RIGHT;}
        else {robot.CORNER_DIRECTION = Robot::LEFT;}
      }
      robot.STATE = Robot::AT_CORNER;
      break;
      
    case Robot::AT_DEADEND:
      robot.readSensors();
      if ( robot.Runs.currentRun == 0 ) {robot.Runs.fastestRun = 1;} // if deadend is reached in first run, second run (left) is correct way
      robot.turnRight(400);
      delay(2250);
      robot.STATE = Robot::SEARCHING;
      break;
      
    case Robot::REVERSING:      // maybe count turns after wrong turn until deadend, so we know when were back to T Junction
      break;
      
    default:    // DISASTER WE ALL WILL DIE...
      break;
  } 
}
  
