
#pragma once


#include <Servo.h>
#include "Sensor.h"
#include "Run.h"

class Robot
{
  private:
  enum state {START, SEARCHING, STOP, AT_CORNER, AT_TJUNCTION, AT_DEADEND, REVERSING} STATE;
  Run RUN[3];
  Servo ServoLeft, ServoRight;
  
  boolean isDeadEnd();
  boolean isCorner();
  boolean hasEnteredMaze();
  boolean isTJunction();
  boolean isFinished();
  boolean isParallel();

  public:
  Robot(void);
  Sensor
  SensorLeftFront,
  SensorLeftBack,
  SensorRightFront,
  SensorRightBack,
  SensorFront,
  SensorBack;

  void startRun();
  void straight();
  void stopBot();
  void turnLeft();
  void turnRight();
  void reverse();
  void decideToTurn();
  void decideTJunctionTurn();
  void decideDeadEnd();
 
};
