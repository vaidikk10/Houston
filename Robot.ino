#include "Robot.h"
#include "Sensor.h"
#include "Run.h"


#define ACCEPTABLE_RANGE 0.6        
#define WMA_SIZE 1
#define SPEED 100
#define SLOW_SPEED 450
#define FRONT_STOPPING_DISTANCE 8

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
  LeftFrontReading = SensorLeftFront->getReading();
  LeftBackReading = SensorLeftBack->getReading();
  RightFrontReading = SensorRightFront->getReading();
  RightBackReading = SensorRightBack->getReading();
//  FrontReading = SensorFront->getReading();   // may not need this here.
//  BackReading = SensorBack->getReading(); 
}


void Robot::straight()
{
  if (SensorFront->getReading() < FRONT_STOPPING_DISTANCE) 
  {
    stopBot();
    return;
  }
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

void Robot::turnLeft()
{
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(1460);    //1460
  ServoRight.writeMicroseconds(1460); 
}


void Robot::turnRight()
{
  ServoRight.attach(12);
  ServoLeft.attach(13);
  ServoLeft.writeMicroseconds(1540); 
  ServoRight.writeMicroseconds(1540);     //1540
}

void Robot::stopBot()
{
  ServoLeft.writeMicroseconds(1500);
  ServoRight.writeMicroseconds(1500);
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
  if (LeftFrontReading < 8 && RightFrontReading < 8 && SensorFront->getReading() < FRONT_STOPPING_DISTANCE)
  {
    return true;  
  }
  return false;  
}

boolean Robot::isCorner()
{
  if (SensorFront->getReading() < (FRONT_STOPPING_DISTANCE+1) && (LeftFrontReading < 10 && RightFrontReading > 14) )
  {
    //right turn
    CORNER_DIRECTION = RIGHT;
    return true;
  } else if (SensorFront->getReading() < (FRONT_STOPPING_DISTANCE+1) && (RightFrontReading < 10 && LeftFrontReading > 14) )
  {
    //left turn
    CORNER_DIRECTION = LEFT;
    return true;
  }
  return false;
}

boolean Robot::hasEnteredMaze()
{
  if (RightFrontReading < 12 && LeftFrontReading < 12 && LeftBackReading && RightBackReading < 12 && STATE == START)
  {
    return true;
  }
  return false;
}

boolean Robot::isTJunction()
{
  if (RightFrontReading > 15 && LeftFrontReading > 15  && SensorFront->getReading() < FRONT_STOPPING_DISTANCE)    // Definately could add more cases here *********************************
  {
    return true;
  }
  return false;
}


boolean Robot::isParallel(enum Direction way)    // USES SENSOR POLLING!!!       ---------- SLOW ---------- ONLY USE IN LOOPS IN LOOP()
{
  double front, back;
  if (way == LEFT)
  {
    front = SensorRightFront->getReading();
    back = SensorRightBack->getReading();
    if (front > 20 || back > 20) { return false; }
    if ((front - ACCEPTABLE_RANGE) < back && back < (front + ACCEPTABLE_RANGE))    // can make more robust *********************
    {
      return true;
    }
  }else if (way == RIGHT)
  {
    front = SensorLeftFront->getReading();
    back = SensorLeftBack->getReading();
    if (front > 20 || back > 20) { return false; }
    if ((front - ACCEPTABLE_RANGE) < back && back < (front + ACCEPTABLE_RANGE))
    {
      return true;
    }
  }
  return false;
}

boolean Robot::isFinished()   // ------------------------------------------------------- DO THIS -------------------------------------------------------
{
  return false;  // idk...
}


void Robot::makeParallel()  
{
  if (SensorFront->getReading() < FRONT_STOPPING_DISTANCE) 
  {
    stopBot();
    return;
  }
//  if ( RightFrontReading > 20 || LeftFrontReading > 20 ) { return; }   // Dont parallel if approaching corner may aswell parallel 
  if (RightFrontReading < 15 && RightBackReading < 15)           // Use right sensors if in range
  {
    if (RightFrontReading < RightBackReading )         
    {
      // Turn slightly left (slow down left servo)
      ServoLeft.writeMicroseconds(2000 - SLOW_SPEED); 
      ServoRight.writeMicroseconds(1000 + SPEED);
//      turnLeft();
    }else if (RightFrontReading > RightBackReading )
    {
      // Turn right
      ServoRight.writeMicroseconds(1000 + SLOW_SPEED); 
      ServoRight.writeMicroseconds(2000 - SPEED);
//      turnRight();
    }
    delay(25);
  }
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
  readSensors();
  if (LeftFrontReading < 15 && LeftBackReading < 15)     // Or use left sensors if in range
  {
    if (LeftFrontReading < LeftBackReading )// || SensorLeftFront->getAvg() < SensorLeftBack->getAvg())
    {
//      turnRight();
      ServoRight.writeMicroseconds(1000 + SLOW_SPEED); 
      ServoLeft.writeMicroseconds(2000 - SPEED);
    }else if (LeftFrontReading > LeftBackReading ) // || SensorLeftFront->getAvg() > SensorLeftBack->getAvg())
    {
      ServoLeft.writeMicroseconds(2000 - SLOW_SPEED); 
      ServoRight.writeMicroseconds(1000 + SPEED);
//      turnLeft();
    }
    delay(25);
  }
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}


void Robot::makeCentre()
{
  if (SensorFront->getReading() < FRONT_STOPPING_DISTANCE) 
  {
    stopBot();
    return;
  }
//  Serial.println("Centring");
  if ( RightFrontReading > 16 || LeftFrontReading > 16 ) { return; }   // Dont centre if approaching corner
  
  if (RightFrontReading > LeftFrontReading && RightBackReading > LeftBackReading)
  {
    ServoRight.writeMicroseconds(1000 + (SLOW_SPEED));
    ServoLeft.writeMicroseconds(2000 - SPEED);
  }else if (RightFrontReading < LeftFrontReading && RightBackReading < LeftBackReading)
  {
    ServoLeft.writeMicroseconds(2000 - (SLOW_SPEED));
    ServoRight.writeMicroseconds(1000 + SPEED);
  }else
  {
    straight();
  }
  delay(10);
}
