
#include "Robot.h"
#include "Sensor.h"
#include "Run.h"


#define ACCEPTABLE_RANGE 10        // arbitrary for now to allow compile
#define WMA_SIZE 1
#define SPEED 100

//***************************** SENSOR PINS *****************************
#define RIGHT_FRONT_TRIG 2
#define RIGHT_FRONT_ECHO 3
#define RIGHT_BACK_TRIG 4
#define RIGHT_BACK_ECHO 5
#define FRONT_TRIG 6
#define FRONT_ECHO 7
#define BACK_TRIG 8
#define BACK_ECHO 9
#define LEFT_FRONT_TRIG 10
#define LEFT_FRONT_ECHO 11
#define LEFT_BACK_TRIG 12
#define LEFT_BACK_ECHO 13
//***********************************************************************


// ----------- IMPORTANT ------------------
// WE MIGHT WANT TO SWAP LEFT BACK TRIG AND ECHO PINS BECAUSE ARDUINO DOCUMENTATION
// STATES PIN 13 IS NOT THE BEST TO WORK WITH AS AN INPUT DUE TO THE LED 
// https://docs.arduino.cc/learn/microcontrollers/digital-pins

Robot::Robot()
{

  
  // ********** Sensor Setup ***********
  SensorLeftFront  = new Sensor(LEFT_FRONT_TRIG, LEFT_FRONT_ECHO, WMA_SIZE);
  SensorLeftBack   = new Sensor(LEFT_BACK_TRIG, LEFT_BACK_ECHO, WMA_SIZE);
  SensorRightFront = new Sensor(RIGHT_FRONT_TRIG, RIGHT_FRONT_ECHO, WMA_SIZE);
  SensorRightBack  = new Sensor(RIGHT_BACK_TRIG, RIGHT_BACK_ECHO, WMA_SIZE);
  SensorFront      = new Sensor(FRONT_TRIG, FRONT_ECHO, WMA_SIZE);
  SensorBack       = new Sensor(BACK_TRIG, BACK_ECHO, WMA_SIZE);

  Serial.print("HELLO WORLD");
  SensorFront->getPins();

  // ********** Servo Setup **********
  ServoRight.attach(12);
  ServoLeft.attach(13);
  STATE = START;
}

Robot::~Robot()
{
  delete SensorLeftFront;
  delete SensorLeftBack;
  delete SensorRightFront;
  delete SensorRightBack;
  delete SensorFront;
  delete SensorBack;
}


void Robot::straight()
{
  if (SensorFront->getReading() < 8 || SensorFront->getAvg() < 10)
  {
  return;
  }
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(1000 + SPEED);
  ServoRight.writeMicroseconds(2000 - SPEED);
}

void Robot::turnLeft()
{
  ServoLeft.writeMicroseconds(1000 + SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}


void Robot::turnRight()
{
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(2000 - SPEED);
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
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

boolean Robot::isDeadEnd()
{
  if (SensorLeftFront->getReading() < 8 && SensorRightFront->getReading() < 8 && SensorFront->getReading() < 8)
  {
    return true;  
  }
  return false;  
}

boolean Robot::isCorner()
{
  if (SensorFront->getReading() < 8 && (SensorLeftFront->getReading() < 8 && SensorRightFront->getReading() > 10))
  {
    //right turn
    return true;
  } else if (SensorFront->getReading() < 8 && (SensorRightFront->getReading() < 8 && SensorLeftFront->getReading() > 10))
  {
    //left turn
    return true;
  }
}

boolean Robot::hasEnteredMaze()
{
  if (SensorRightFront->getReading() < 8 && SensorLeftFront->getReading() < 8 && STATE == START)
  {
    STATE = SEARCHING;
    return true;
  }
  return false;
}

boolean Robot::isTJunction()
{
  if (SensorRightFront->getReading() > 10 && SensorLeftFront->getReading() > 10 && SensorFront->getReading() < 8)    // Definately could add more cases here *********************************
  {
    return true;
  }
  return false;
}


boolean Robot::isParallel()
{
  if (SensorRightFront->getReading() - ACCEPTABLE_RANGE < SensorRightBack->getReading() < SensorRightFront->getReading() + ACCEPTABLE_RANGE)    // can make more robust *********************
  {
    return true;
  }
  return false;
}

boolean Robot::isFinished()   // ------------------------------------------------------- DO THIS -------------------------------------------------------
{
  return true;
}

void Robot::makeParallel()
{
  if (SensorRightFront->getReading() < 15 && SensorRightBack->getReading() < 15)
  {
    if (SensorRightFront->getReading() < SensorRightBack->getReading() || SensorRightFront->getAvg() < SensorRightBack->getAvg())          
    {
      // Turn slightly left (slow down left servo)
      ServoLeft.writeMicroseconds(1000 + (SPEED*2)); 
      delay(10);
    }else if (SensorRightFront->getReading() > SensorRightBack->getReading() || SensorRightFront->getAvg() > SensorRightBack->getAvg())
    {
      ServoRight.writeMicroseconds(2000 - (SPEED*2)); 
      delay(10);
    }
  }else if (SensorLeftFront->getReading() < 15 && SensorLeftBack->getReading() < 15)
  {
    if (SensorLeftFront->getReading() < SensorLeftBack->getReading() || SensorLeftFront->getAvg() < SensorLeftBack->getAvg())
    {
      ServoRight.writeMicroseconds(2000 - (SPEED*2)); 
      delay(10);
    }else if (SensorLeftFront->getReading() > SensorLeftBack->getReading() || SensorLeftFront->getAvg() > SensorLeftBack->getAvg())
    {
      ServoLeft.writeMicroseconds(1000 + (SPEED*2)); 
      delay(10);
    }
  }
}
