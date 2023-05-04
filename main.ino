//Front Sensor Wall Detection and Movemnt
//Sends sensor data to python using HC-06


#include <Servo.h>
#include <SoftwareSerial.h>
#include "Robot.h"

#define TURNING_SPEED 40   // 40
#define SLOW_CORNER 25


SoftwareSerial HC06(12, 13); //HC06-TX Pin 10, HC06-RX to Arduino Pin 11


Robot *robot;
void ButtonPressed_EXTI0(void);    // ISR
int buttonInterrupt = false;

unsigned long LEDflash_time;


void setup() {
  LEDflash_time = millis();


  pinMode(4,OUTPUT);       //LF
  pinMode(5,INPUT);
  pinMode(6,OUTPUT);      //LB ***** Needs resoldering
  pinMode(7,INPUT);
  pinMode(15,OUTPUT);      //RF
  pinMode(14,INPUT);
  pinMode(10,OUTPUT);      //RB
  pinMode(11,INPUT);
  pinMode(17,OUTPUT);      //F
  pinMode(16,INPUT);

  pinMode(A5, OUTPUT);   //LED0
  pinMode(A4, OUTPUT);   //LED1
  pinMode(2, OUTPUT);   //LED2
  pinMode(3, INPUT);   //Button
  
  HC06.begin(9600); //Baudrate 9600
  Serial.begin(9600);
//  prev = millis();   // Time (ms) since arduino started
  
  Serial.println("SERIAL HAS BEGUN\n"); // print some text in Serial Monitor
  attachInterrupt(digitalPinToInterrupt(3), ButtonPressed_EXTI0, FALLING);      // Declare ButtonPressed_EXTI0_Handler as ISR on pin 2
  robot = new Robot();    // call constructor on robot

}



void loop() 
{
  if ( buttonInterrupt ) ButtonPressed_EXTI0_Handler(robot);    // check if interrupt

  robot->readSensors();
  switch (robot->STATE)
  {
    case Robot::BEFORE_RUN:
      if ( (millis() - LEDflash_time) > 1000 )    // if its been 1.5s since last flash
      {
        LEDflash_time = millis();
        robot->LED_flash();
      }
      break;
    case Robot::START:   //  *** OUTSIDE OF MAZE ***
      robot->straight();
      if (robot->hasEnteredMaze())
      robot->STATE = Robot::SEARCHING;
      break;
    
    case Robot::SEARCHING:
       robot->readSensors();
       
       robot->straight();
       robot->makeParallel();
       robot->makeCentre();
       // if (robot->isFinished()) robot->STATE = Robot::FINISHED;
      
       if (robot->SensorFront->getReading() < 8) // START CHECKING FOR CORNERS WHEN WITHIN 10CM OF WALL  
       {
        if (robot->isCorner() )
        { 
          // robot->STATE = Robot::AT_CORNER; 
          if (robot->CORNER_DIRECTION == Robot::LEFT)
          {
//            robot->turnLeft(TURNING_SPEED);
//            delay(75);
            robot->turnLeft90();
          }
          else if (robot->CORNER_DIRECTION == Robot::RIGHT)
          {
//            robot->turnRight(TURNING_SPEED);
//            delay(75);
              robot->turnRight90();
          }
          robot->STATE = Robot::SEARCHING;
        }
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
        if (robot->isParallel(Robot::LEFT) && robot->isParallel(Robot::LEFT) && robot->isParallel(Robot::LEFT) && robot->isParallel(Robot::LEFT))      // 2 in a row needed. (end infinite loop when parallel)
        {
          robot->STATE = Robot::SEARCHING;
          // robot->straight();
        }
        if (robot->RightFrontReading <= robot->RightBackReading)
        {
          robot->turnLeft(SLOW_CORNER);
        }else
        {
          robot->turnLeft(75);
        }
      }
      else if (robot->CORNER_DIRECTION == Robot::RIGHT)
      {
        if (robot->isParallel(Robot::RIGHT) && robot->isParallel(Robot::RIGHT) && robot->isParallel(Robot::RIGHT) && robot->isParallel(Robot::RIGHT))
        {
          robot->STATE = Robot::SEARCHING;
          // robot->straight();
        }
        if (robot->LeftFrontReading <= robot->LeftBackReading)
        {
          robot->turnRight(SLOW_CORNER);
        }else
        {
          robot->turnRight(75);
        }
      }
      break;

            
    case Robot::AT_TJUNCTION:
      if ( robot->Runs.currentRun == 0 )
      {
        robot->CORNER_DIRECTION = Robot::RIGHT;
        //robot->turnRight(TURNING_SPEED);
      }else if ( robot->Runs.currentRun == 1 ) 
      {
        robot->CORNER_DIRECTION = Robot::LEFT; 
        //robot->turnLeft(TURNING_SPEED);
      } else if ( robot->Runs.currentRun == 2 )
      {
        if ( robot->Runs.fastestRun == 0 ) 
        {
          robot->CORNER_DIRECTION = Robot::RIGHT;
          //robot->turnRight(TURNING_SPEED);
        }else 
        {
          robot->CORNER_DIRECTION = Robot::LEFT;
          //robot->turnLeft(TURNING_SPEED);
        }
      }
      // delay(100);
      if(robot->CORNER_DIRECTION == Robot::LEFT)
      {
        robot->turnLeft90();
      }
      else if (robot->CORNER_DIRECTION == Robot::RIGHT)
      {
          robot->turnRight90();
      }
  robot->STATE = Robot::SEARCHING;
      break;
      
    case Robot::AT_DEADEND:
      if ( robot->Runs.currentRun == 0 ) {robot->Runs.fastestRun = 1;} // if deadend is reached in first run, second run (left) is correct way
      robot->turnRight(TURNING_SPEED);
      delay(2450);
      robot->STATE = Robot::SEARCHING;
      break;

    case Robot::REVERSING:      // maybe count turns after wrong turn until deaedend, so we know when were back to T Junction
      break;

    case Robot::FINISHED:
      if (robot->isFinished())
      {
        robot->STATE = Robot::STOP;
      }

    case Robot::LAST_RUN_FINISHED:
      robot->stopBot();
      // turn LEDs off
      digitalWrite(2,  LOW);
      digitalWrite(18, LOW);
      digitalWrite(19, LOW);
      break;
      
    default:
      break;
  } 
}


void ButtonPressed_EXTI0(void)
{
  buttonInterrupt = true;
}
