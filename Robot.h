
#ifndef ROBOT
#define ROBOT


#include <Servo.h>
#include "Sensor.h"
#include "Run.h"

class Robot
{
  private:
  Servo ServoLeft, ServoRight;
  
  public:
  struct LED
  {
    bool state;
    int pin;
  } RedLED, YellowLED, GreenLED;
  struct  
  {
    Run individualRun[3];
    int currentRun = 0;
    int fastestRun = 0;   // fastest run is first (right turn) by default
  } Runs;
  enum state {BEFORE_RUN=0, START, SEARCHING, STOP, AT_CORNER, AT_TJUNCTION, FINISHED, AT_DEADEND, REVERSING, LAST_RUN_FINISHED} STATE;
  enum Direction {LEFT = 1, RIGHT} CORNER_DIRECTION;

  Sensor
  *SensorLeftFront,
  *SensorLeftBack,
  *SensorRightFront,
  *SensorRightBack,
  *SensorFront;

  double
  LeftFrontReading,
  LeftBackReading,
  RightFrontReading,
  RightBackReading,
  FrontReading;

  Robot();
  ~Robot();
  

  void startRun();
  void straight();
  void stopBot();
  void turnLeft(short);
  void turnRight(short);
  void reverse();
  void decideToTurn();
  void decideTJunctionTurn();
  void decideDeadEnd();

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
  void LED_flash();
  friend void ButtonPressed_EXTI0_Handler(Robot*);
};

#endif
