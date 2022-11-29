

#include "Sensor.h"


Sensor::Sensor(int trigPin, int echoPin)
{
  this->trigPin = trigPin;
  this->echoPin = echoPin;
}

double Sensor::getReading()
{
  double duration, distance;
  digitalWrite(trigPin, LOW);   // reset to low position then pulse on to trigger
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  if (duration == 0) {
    Serial.print("Sensor Disconnected [Trig, Echo] : ");
    Serial.print(trigPin);
    Serial.print(", ");
    Serial.println(echoPin);
    return -1;
  } else {
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    if(distance > 150){
      distance = 150;
    }
  }
  delay(10);
  return distance;

}
