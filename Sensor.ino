#include "Sensor.h"




Sensor::Sensor(int trigPin, int echoPin)
{
  this->trigPin = trigPin;
  this->echoPin = echoPin;
}


//Sensor::~Sensor()
//{
//  delete sonar;
//}





double Sensor::getReading()
{
  double duration, distance;
  digitalWrite(trigPin, LOW);   // reset to low position then pulse on to trigger
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 75000);     // Time out at 75 ms, this will return 0
  if (duration == 0) 
  {
    Serial.print("Sensor Disconnected [Trig, Echo] : ");
    Serial.print(trigPin);
    Serial.print(", ");
    Serial.println(echoPin);
    // MAYBE PUT AN AVERAGE VALUE IN HERE FOR THE PAST ELEMENT WMA ARRAY???
    if (trigPin == FRONT_TRIG) return 0;  // to make sure the bot will stop!
    return 150;  
  } else 
  {
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    if(distance > 150){
      distance = 150;
    }
  }
  
  delay(2);
  if (trigPin == 6 || trigPin == 14)    // remove this after testing 
    {
      Serial.print("Sensor: ");
      Serial.print(trigPin);
      Serial.print(" is: ");
      Serial.println(distance);
    }
  return distance;

}
