#include "Robot.h"
#include "Sensor.h"
#include "Run.h"


#define ACCEPTABLE_RANGE 0.75        // arbitrary for now to allow compile
#define WMA_SIZE 1
#define SPEED 100
#define SLOW_SPEED 450

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
#define LEFT_BACK_TRIG 14    // A0 
#define LEFT_BACK_ECHO 15    // A1 
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

void Robot::readSensors()
{
  robot->LeftFrontReading = robot->SensorLeftFront->getReading();
  robot->LeftBackReading = robot->SensorLeftBack->getReading();
  robot->RightFrontReading = robot->SensorRightFront->getReading();
  robot->RightBackReading = robot->SensorRightBack->getReading();
//  robot->FrontReading = robot->SensorFront->getReading();   // may not need this here.
//  robot->BackReading = robot->SensorBack->getReading(); 
}


void Robot::straight()
{
  if (SensorFront->getReading() < 10 || SensorFront->getAvg() < 10)
  {
    Serial.println("STOP!");
    stopBot();
    return;
  }
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

int Robot::turnLeft()                     // could make the returns more robust.
{
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(1460); //1000 + SPEED);
  ServoRight.writeMicroseconds(1460); //1000 + SPEED);
  if (SensorFront->getReading() > 12 && isParallel()) return 1;  // return 1 when complete
  return 0;       // when not complete
}


int Robot::turnRight()
{
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(1540); // 2000 - SPEED);
  ServoRight.writeMicroseconds(1540); // 2000 - SPEED);
  if (SensorFront->getReading() > 12 && isParallel()) return 1;
  return 0;
}

void Robot::stopBot()
{
  // while (!robot->isParallel() ) makeParallel();
  ServoLeft.detach();
  ServoRight.detach();
  STATE = SEARCHING;
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
  if (LeftFrontReading < 8 && RightFrontReading < 8 && SensorFront->getReading() < 8)
  {
    return true;  
  }
  return false;  
}

boolean Robot::isCorner()
{
  if (SensorFront->getReading() < 8 && (LeftFrontReading < 10 && RightFrontReading > 18))
  {
    //right turn
    CORNER_DIRECTION = RIGHT;
    return true;
  } else if (SensorFront->getReading() < 12 && (SensorRightFront->getReading() < 10 && SensorLeftFront->getReading() > 18))
  {
    //left turn
    CORNER_DIRECTION = LEFT;
    return true;
  }
  return false;
}

boolean Robot::hasEnteredMaze()
{
  if (RightFrontReading < 12 && LeftFrontReading < 12 && STATE == START)
  {
    return true;
  }
  return false;
}

boolean Robot::isTJunction()
{
  if (RightFrontReading > 10 && LeftFrontReading > 10 && SensorFront->getReading() < 8)    // Definately could add more cases here *********************************
  {
    return true;
  }
  return false;
}


boolean Robot::isParallel()
{
  if ((RightFrontReading - ACCEPTABLE_RANGE) < RightBackReading && RightBackReading < (RightFrontReading + ACCEPTABLE_RANGE))    // can make more robust *********************
  {
    return true;
  }else if ((LeftFrontReading - ACCEPTABLE_RANGE) < LeftBackReading && LeftBackReading < (LeftFrontReading + ACCEPTABLE_RANGE))
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
  if (RightFrontReading < 15 && RightBackReading < 15)           // Use right sensors if in range
  {
    if (RightFrontReading < RightBackReading )// || SensorRightFront->getAvg() < SensorRightBack->getAvg())          
    {
      // Turn slightly left (slow down left servo)
      ServoLeft.writeMicroseconds(2000 - SLOW_SPEED); 
      ServoRight.writeMicroseconds(1000 + SPEED);
//      turnLeft();
      delay(10);
    }else if (RightFrontReading > RightBackReading ) // || SensorRightFront->getAvg() > SensorRightBack->getAvg())
    {
      // Turn right
      ServoRight.writeMicroseconds(1000 + SLOW_SPEED); 
      ServoRight.writeMicroseconds(2000 - SPEED);
//      turnRight();
      delay(10);
    }
  }
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
  if (LeftFrontReading < 15 && LeftBackReading < 15)     // Or use left sensors if in range
  {
    if (LeftFrontReading < LeftBackReading )// || SensorLeftFront->getAvg() < SensorLeftBack->getAvg())
    {
//      turnRight();
      ServoRight.writeMicroseconds(1000 + SLOW_SPEED); 
      ServoLeft.writeMicroseconds(2000 - SPEED);
      delay(10);
    }else if (LeftFrontReading > LeftBackReading ) // || SensorLeftFront->getAvg() > SensorLeftBack->getAvg())
    {
      ServoLeft.writeMicroseconds(2000 - SLOW_SPEED); 
      ServoRight.writeMicroseconds(1000 + SPEED);
//      turnLeft();
      delay(10);
    }
  }
}


void Robot::makeCentre()
{
//  Serial.println("Centring");
  if (SensorFront->getReading() < 8 ) stopBot();
  if (RightFrontReading > LeftFrontReading && RightBackReading > LeftBackReading)
  {
    ServoRight.writeMicroseconds(1000 + (460));
    ServoLeft.writeMicroseconds(2000 - SPEED);
//    while ( !isParallel() )
//    {
//      if (SensorFront->getReading() < 10) return;  
//    }
  }else if (RightFrontReading < LeftFrontReading && RightBackReading < LeftBackReading)
  {
    ServoLeft.writeMicroseconds(2000 - (460));
    ServoRight.writeMicroseconds(1000 + SPEED);
//    while ( !isParallel() )
//    {
//      if (SensorFront->getReading() < 10) return;
//    }
  }else
  {
    straight();
  }
}
