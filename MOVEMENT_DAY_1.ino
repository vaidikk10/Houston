
#include <Servo.h>
int LED_PIN = 13;

Servo ServoLeft;      // 1000 - 1499 to go forward
Servo ServoRight;     // 1501 - 2000 to go forward


unsigned short speed;     //<- 0-100


void setup()
{
 speed = 50;
  pinMode(LED_PIN, OUTPUT);
  ServoLeft.attach(12);
  ServoRight.attach(13);
}

void loop()
{
  straight();
  delay(2000);
  stopBot();
  delay(2000);
  right();
  delay(2000);
  stopBot();
  delay(2000);
  left();
  delay(2000);
  straight();
  delay(2000);
}


void straight(){
  ServoLeft.writeMicroseconds(1000);
  ServoRight.writeMicroseconds(2000);
}

void right(){
  ServoLeft.writeMicroseconds(1500 - speed);
  ServoRight.writeMicroseconds(1500);
}

void left(){
  ServoLeft.writeMicroseconds(1500);
  ServoRight.writeMicroseconds(1500 + speed);
}

void stopBot(){
  ServoLeft.writeMicroseconds(1500);
  ServoRight.writeMicroseconds(1500);
}
