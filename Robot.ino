
#define ACCEPTABLE_RANGE 10

#include "Robot.h"
#include "Sensor.h"
#include "Run.h"


void Robot::straight()
{
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

void Robot::turnLeft()
{
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(2000 - SPEED);
}


void Robot::turnRight()
{
  ServoLeft.writeMicroseconds(1000 + SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

void Robot::stopBot()
{
  ServoLeft.detach();
  ServoRight.detach();
}

void Robot::reverse()
{
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(1000 + SPEED);
  ServoRight.writeMicroseconds(2000 - SPEED);
}

boolean Robot::isDeadEnd()
{
  if (SensorLeftFront.getReading() < 8 && SensorRightFront.getReading() < 8 && SensorFront.getReading() < 8)
  {
    return true;  
  }
  return false;  
}

boolean Robot::isCorner()
{
  if (SensorFront.getReading() < 8 && (SensorLeftFront.getReading() < 8 && SensorRightFront.getReading() > 10))
  {
    //right turn
    return true;
  } else if (SensorFront.getReading() < 8 && (SensorRightFront.getReading() < 8 && SensorLeftFront.getReading() > 10))
  {
    //left turn
    return true;
  }
}

boolean Robot::hasEnteredMaze()
{
  if (SensorRightFront.getReading() < 8 && SensorLeftFront.getReading() < 8 && STATE == START)
  {
    STATE = SEARCHING;
    return true;
  }
  return false;
}

boolean Robot::isTJunction()
{
  if (SensorRightFront.getReading() > 10 && SensorLeftFront.getReading() > 10 && SensorFront.getReading() < 8)    // Definately could add more cases here *********************************
  {
    return true;
  }
  return false;
}


boolean Robot::isParallel()
{
  if (SensorRightFront.getReading() - ACCEPTABLE_RANGE < SensorRightBack.getReading() < SensorRightFront.getReading() + ACCEPTABLE_RANGE)    // can make more robust *********************
  {
    return true;
  }
  return false;
}

boolean Robot::isFinished()
{
  
}
