
#pragma once


#include <Servo.h>
#include "Sensor.h"
#include "Run.h"

class Robot
{
  private:

  
  Servo ServoLeft, ServoRight;
  
  


  public:
  struct  
  {
    Run individualRun[3];
//    Run * currentRun;
    int currentRun = 0;
    int fastestRun = 0;   // fastest run is first (right turn) by default
  } Runs;
  enum state {START, SEARCHING, STOP, AT_CORNER, AT_TJUNCTION, AT_DEADEND, REVERSING} STATE;
  enum Direction {LEFT = 1, RIGHT} CORNER_DIRECTION;
  Robot();
  ~Robot();
  
  Sensor
  *SensorLeftFront,
  *SensorLeftBack,
  *SensorRightFront,
  *SensorRightBack,
  *SensorFront,
  *SensorBack;

  double
  LeftFrontReading,
  LeftBackReading,
  RightFrontReading,
  RightBackReading,
  FrontReading,
  BackReading;

  void startRun();
  void straight();
  void stopBot();
  void turnLeft();
  void turnRight();
  void reverse();
  void decideToTurn();
  void decideTJunctionTurn();
  void decideDeadEnd();

// ********** MADE THESE PUBLIC! **********
  boolean isDeadEnd();
  boolean isCorner();
  boolean hasEnteredMaze();
  boolean isTJunction();
  boolean isFinished();
  boolean isParallel(enum Direction);

  // ********** NEWLY ADDED ********** 
  void makeParallel();
  void makeCentre();
  void readSensors();
  void ButtonPressed_EXTI0_Handler();
};
