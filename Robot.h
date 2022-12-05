
#pragma once


#include <Servo.h>
#include "Sensor.h"
#include "Run.h"

class Robot
{
  private:
  Run RUN[3];
  Servo ServoLeft, ServoRight;
  
  


  public:
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

  void startRun();
  void straight();
  void stopBot();
  int turnLeft();
  int turnRight();
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
  boolean isParallel();

  // ********** NEWLY ADDED ********** 
  void makeParallel();
  void makeCentre();
 
};
