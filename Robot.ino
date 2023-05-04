#include "Robot.h"
#include "Sensor.h"
#include "Run.h"


// It is useful to increase the acceptable range if you want to also want to increase the no of consecutive parallel readings to stop turning.
#define ACCEPTABLE_RANGE 0.55
#define SPEED 400          // Pretty much max speed    -- changed to 400 from 100
#define SLOW_SPEED 460     // changed from 450 to 425 to reduce jankyness
#define FRONT_STOPPING_DISTANCE 7     // 6 to 4.5 to // 5.5 to 6.5 to 6 to 7
#define TURNING_SPEED 40

//***************************** BUTTON SYSTEMS PINS *****************************
/* SWITCH PIN : 3
 *  LED0      : A5
 *  LED1      : A4
 *  LED2      : 2
 */

#define BUTTON_PIN 3
#define LED2 2     //        -- Green
#define LED1 18    // A4     -- Red
#define LED0 19    // A5     -- Yellow


//***************************** SENSOR PINS *****************************
#define RIGHT_FRONT_TRIG 15  
#define RIGHT_FRONT_ECHO 14  
#define RIGHT_BACK_TRIG 10
#define RIGHT_BACK_ECHO 11
#define FRONT_TRIG 17
#define FRONT_ECHO 16
#define LEFT_FRONT_TRIG 4
#define LEFT_FRONT_ECHO 5
#define LEFT_BACK_TRIG 6     
#define LEFT_BACK_ECHO 7     
//***********************************************************************


Robot::Robot()
{
  
  // ********** Sensor Setup ***********
  SensorLeftFront  = new Sensor(LEFT_FRONT_TRIG, LEFT_FRONT_ECHO);
  SensorLeftBack   = new Sensor(LEFT_BACK_TRIG, LEFT_BACK_ECHO);
  SensorRightFront = new Sensor(RIGHT_FRONT_TRIG, RIGHT_FRONT_ECHO);
  SensorRightBack  = new Sensor(RIGHT_BACK_TRIG, RIGHT_BACK_ECHO);
  SensorFront      = new Sensor(FRONT_TRIG, FRONT_ECHO);


  // ********** Servo Setup **********
  ServoRight.attach(12);
  ServoLeft.attach(13);
  STATE = BEFORE_RUN;

  // ********** LED Setup **********
  YellowLED = {false, LED0};
  RedLED = {false, LED1};
  GreenLED = {false, LED2};
}

Robot::~Robot()
{
  delete SensorLeftFront;
  delete SensorLeftBack;
  delete SensorRightFront;
  delete SensorRightBack;
  delete SensorFront;
}

void Robot::turnRight90(void)
  {
      if (!ServoLeft.attached() && !ServoRight.attached())
      {
        ServoRight.attach(12);
        ServoLeft.attach(13);
      }
      ServoLeft.writeMicroseconds(1500 + 40);    // 40 to 45
      ServoRight.writeMicroseconds(1500 + 40);
      delay(1150);
  }
void Robot::turnLeft90(void)
  {
      if (!ServoLeft.attached() && !ServoRight.attached())
      {
        ServoRight.attach(12);
        ServoLeft.attach(13);
      }
      ServoLeft.writeMicroseconds(1500 - 40);
      ServoRight.writeMicroseconds(1500 - 40);
      delay(1150);
  }


void Robot::LED_flash()
{
    switch (Runs.currentRun)
      {
      case 0:
        GreenLED.state = !(GreenLED.state & 0x1); // flip state of LED
        digitalWrite(GreenLED.pin, GreenLED.state);
        digitalWrite(RedLED.pin, LOW);
        digitalWrite(YellowLED.pin, LOW);
        break;
      case 1:
        RedLED.state = !(RedLED.state & 0x1);
        GreenLED.state = RedLED.state; 
        digitalWrite(RedLED.pin, RedLED.state);
        digitalWrite(GreenLED.pin, GreenLED.state);
        digitalWrite(YellowLED.pin, LOW);
        break;
      case 2:
        YellowLED.state = !(YellowLED.state & 0x1);
        RedLED.state = YellowLED.state;
        GreenLED.state = YellowLED.state; 
        digitalWrite(YellowLED.pin, YellowLED.state);
        digitalWrite(RedLED.pin, RedLED.state);
        digitalWrite(GreenLED.pin, GreenLED.state);
        break;
      }
}

void Robot::readSensors()
{
  LeftFrontReading = SensorLeftFront->getReading();
  LeftBackReading = SensorLeftBack->getReading();
  RightFrontReading = SensorRightFront->getReading();
  RightBackReading = SensorRightBack->getReading();
//  FrontReading = SensorFront->getReading();   // may not need this here.
}


void Robot::straight()
{
  if (SensorFront->getReading() < FRONT_STOPPING_DISTANCE) 
  {
    stopBot();
    return;
  }
  if (!ServoLeft.attached() && !ServoRight.attached())
  {
    ServoRight.attach(12);
    ServoLeft.attach(13);
  }
  ServoLeft.writeMicroseconds(2000 - SPEED);
  ServoRight.writeMicroseconds(1000 + SPEED);
}

void Robot::turnLeft(short turning_speed)
{
  if (!ServoLeft.attached() && !ServoRight.attached())
  {
    ServoRight.attach(12);
    ServoLeft.attach(13);
  }
  ServoLeft.writeMicroseconds(1500 - turning_speed);
  ServoRight.writeMicroseconds(1500 - turning_speed);
//  ServoLeft.writeMicroseconds(1000 + TURNING_SPEED);    //1460   - slow turning speed is good
//  ServoRight.writeMicroseconds(1000 + TURNING_SPEED); 
}


void Robot::turnRight(short turning_speed)
{
  //turning speed from ~ 40 to 100, 40 being slowest 
  if (!ServoLeft.attached() && !ServoRight.attached())
  {
    ServoRight.attach(12);
    ServoLeft.attach(13);
  }
  ServoLeft.writeMicroseconds(1500 + turning_speed);
  ServoRight.writeMicroseconds(1500 + turning_speed);
//  ServoLeft.writeMicroseconds(2000 - TURNING_SPEED); 
//  ServoRight.writeMicroseconds(2000 - TURNING_SPEED);     //1540
}

void Robot::stopBot()
{
  ServoLeft.writeMicroseconds(1500);
  ServoRight.writeMicroseconds(1500);
  ServoLeft.detach();
  ServoRight.detach();
  // STATE = SEARCHING;    // Could lead to circular loop if not careful - probably change this
}

void Robot::reverse()
{
  if (!ServoLeft.attached() && !ServoRight.attached())
  {
    ServoRight.attach(12);
    ServoLeft.attach(13);
  }
  ServoLeft.writeMicroseconds(1000 + SPEED);
  ServoRight.writeMicroseconds(2000 - SPEED);
}

boolean Robot::isDeadEnd()
{
  if (LeftFrontReading < 10 && RightFrontReading < 10 && SensorFront->getReading() < FRONT_STOPPING_DISTANCE)
  {
    return true;  
  }
  return false;  
}

boolean Robot::isCorner()
{
  if (SensorFront->getReading() < (FRONT_STOPPING_DISTANCE) && (LeftFrontReading < 14 && RightFrontReading > 15) )    // 12 to 14
  {
    //right turn
    CORNER_DIRECTION = RIGHT;
    return true;
  } else if (SensorFront->getReading() < (FRONT_STOPPING_DISTANCE) && (RightFrontReading < 14 && LeftFrontReading > 15) )  // 12 to 14
  {
    //left turn
    CORNER_DIRECTION = LEFT;
    return true;
  }
  return false;
}

boolean Robot::hasEnteredMaze()
{
  if ((RightFrontReading < 12 && RightBackReading) || (LeftFrontReading < 12 && LeftBackReading)  < 12 && STATE == START)
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


boolean Robot::isParallel(enum Direction way)    // POLLS A FEW SENSORS SO QUITE SLOW!
{
  double front, back;
  if (way == LEFT)
  {
    front = SensorRightFront->getReading();
    back = SensorRightBack->getReading();
    if (front > 20 || back > 20) { return false; }
    if ((front - ACCEPTABLE_RANGE) < back && back < (front + ACCEPTABLE_RANGE))
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
{                             // BUTTONS COULD ASSIST WITH THIS.  ----- CHECK FLAG SET BY BUTTON INTERRUPT ----- 
                              // THIS INTERRUPT COULD BE IN RUN (IT NEEDS TO BE A STATIC CLASS METHOD) STATIC TO ALLOW IT TO EXIST WITHOUT AN INSTANCE
  if ( LeftFrontReading > 20 && LeftBackReading > 20 && RightFrontReading > 20 && RightBackReading > 20 && SensorFront->getReading() > 20 ) return true;
  return false;  // idk...
}


void Robot::makeParallel()  
{
  if (SensorFront->getReading() < FRONT_STOPPING_DISTANCE) 
  {
    stopBot();
    return;
  }
  if (RightFrontReading < 10 && RightBackReading < 10)           // Use right sensors if in range     -- 10 to 8
  {
    if ( RightFrontReading < RightBackReading )         
    {
      // Turn slightly left (slow down left servo)
      ServoLeft.writeMicroseconds(2000 - SLOW_SPEED); 
      ServoRight.writeMicroseconds(1000 + SPEED);
//      turnLeft
    }else if ( RightFrontReading > RightBackReading )
    {
      // Turn right
      ServoRight.writeMicroseconds(1000 + SLOW_SPEED); 
      ServoLeft.writeMicroseconds(2000 - SPEED);
//      turnRight
    }
  }
  readSensors();
  /*else*/ if (LeftFrontReading < 10 && LeftBackReading < 10)     // Or use left sensors if in range
  {
    if (LeftFrontReading < LeftBackReading )
    {
//      turnRight
      ServoRight.writeMicroseconds(1000 + SLOW_SPEED); 
      ServoLeft.writeMicroseconds(2000 - SPEED);
    }else if (LeftFrontReading > LeftBackReading )
    {
//      turnLeft
      ServoLeft.writeMicroseconds(2000 - SLOW_SPEED); 
      ServoRight.writeMicroseconds(1000 + SPEED);
    }
    delay(20);
  }
}


void Robot::makeCentre()
{
  if (SensorFront->getReading() < FRONT_STOPPING_DISTANCE) 
  {
    stopBot();
    return;
  }
  if ((RightBackReading - RightFrontReading) > 3) 
  {
    makeParallel();
    return;
  }else if ((LeftBackReading - LeftFrontReading) > 3) 
  {
    makeParallel();
    return;
  }
  
// return out of all functions between dashes
// -------------------------------------------------------------------------------------------------
  if ( RightFrontReading > 20 && LeftFrontReading < 5 ) 
  {  // right 
    ServoRight.writeMicroseconds(1000 + (SLOW_SPEED));
    ServoLeft.writeMicroseconds(2000 - SPEED);
    return;    
  } else if ( LeftFrontReading > 20 && RightFrontReading < 5 )
  {  // left
    ServoLeft.writeMicroseconds(2000 - (SLOW_SPEED));
    ServoRight.writeMicroseconds(1000 + SPEED);
    return;    
  } else if ( RightFrontReading > 20 && (5 < LeftFrontReading && LeftFrontReading < 10))    
  {  // left
    ServoLeft.writeMicroseconds(2000 - (SLOW_SPEED));
    ServoRight.writeMicroseconds(1000 + SPEED);
    return;    
  } else if ( LeftFrontReading > 20 && (5 < RightFrontReading && RightFrontReading < 10))      
  {  // right
    ServoRight.writeMicroseconds(1000 + (SLOW_SPEED));
    ServoLeft.writeMicroseconds(2000 - SPEED);
    return;
  } else if ( RightFrontReading > 20 || LeftFrontReading > 20 ) { return; }
  // If any of the above is true we dont want to centre past this point
  // -------------------------------------------------------------------------------------------------

  
  if (RightFrontReading > LeftFrontReading && RightBackReading > LeftBackReading)
  {   // right 
    ServoRight.writeMicroseconds(1000 + (SLOW_SPEED));
    ServoLeft.writeMicroseconds(2000 - SPEED);
  }else if (RightFrontReading < LeftFrontReading && RightBackReading < LeftBackReading)
  {  // left
    ServoLeft.writeMicroseconds(2000 - (SLOW_SPEED));
    ServoRight.writeMicroseconds(1000 + SPEED);
  }else 
  {
    straight();
  }
}


// ******************************************************* INTERRUPT ***************************************************

void ButtonPressed_EXTI0_Handler (Robot* bot)    // This is a friend function and there ISR for the button press
{
  buttonInterrupt = false; 
  static long last_time;
  if ( (millis() - last_time) < 500 )
  {
    Serial.println(millis()-last_time);
    last_time = millis();
    return;
  }
  
  
  bot->stopBot();
  if ( bot->STATE == Robot::BEFORE_RUN ) 
    {
      bot->STATE = Robot::START;
      bot->ServoLeft.attach(13);
      bot->ServoRight.attach(12);
      switch (bot->Runs.currentRun)
        {
        case 0:
          digitalWrite(bot->GreenLED.pin, HIGH);
          digitalWrite(bot->RedLED.pin, LOW);
          digitalWrite(bot->YellowLED.pin, LOW);
          break;
        case 1:
          digitalWrite(bot->RedLED.pin, HIGH);
          digitalWrite(bot->GreenLED.pin, HIGH);
          digitalWrite(bot->YellowLED.pin, LOW);
          break;
        case 2:
          digitalWrite(bot->YellowLED.pin, HIGH);
          digitalWrite(bot->RedLED.pin, HIGH);
          digitalWrite(bot->GreenLED.pin, HIGH);
          break;
        }
    }else if (bot->STATE == Robot::LAST_RUN_FINISHED)
    {
      bot->Runs.currentRun = 0;
      bot->STATE = Robot::BEFORE_RUN;
    }else
    {
     bot->ServoLeft.detach();
     bot->ServoRight.detach();
     digitalWrite(bot->GreenLED.pin, LOW);
     digitalWrite(bot->RedLED.pin, LOW);
     digitalWrite(bot->YellowLED.pin, LOW);
      if ( bot->Runs.currentRun == 2 )  // final run complete
      {
        bot->STATE = Robot::LAST_RUN_FINISHED;
        return;
      }
      bot->STATE = Robot::BEFORE_RUN;
      (bot->Runs.currentRun)++;
    }
}
