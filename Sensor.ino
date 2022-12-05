#include "Sensor.h"


Sensor::~Sensor()
{
  delete sonar;
}


Sensor::Sensor(int trigPin, int echoPin, int EWMA_size)
{
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  _EWMA_size = EWMA_size;
  pastElements = (double*)malloc(EWMA_size*sizeof(double));
  _arrayCursor = 0;       // index of first element of circular queue of distance data 
  _avgActive = 0;
  sonar = new NewPing(trigPin, echoPin, 150);          // MAX DISTANCE is final argument
}

double Sensor::getReading2()
{
    double duration, distance;
    if (sonar->ping())
    {
      distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
      if(distance > 150)
      {
        distance = 150;
      }
    return distance;

    }else
    {
    Serial.print("Sensor Disconnected [Trig, Echo] : ");
    Serial.print(trigPin);
    Serial.print(", ");
    Serial.println(echoPin);
    return -1;  
    }
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
    // MAYBE PUT AN AVERAGE VALUE IN HERE FOR THE PAST ELEMENT WMA ARRAY???
    return -1;  
  } else {
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    if(distance > 150){
      distance = 150;
    }
  }
  
  // add to pastElements (circular FIFO data structure that pops oldest value when array is filled + 1)
//  pastElements[_arrayCursor] = distance;
//  _arrayCursor++;
//  if (_arrayCursor > _EWMA_size-1) 
//  {
//    _arrayCursor = 0;
//    _avgActive = true;
//  }
  delay(10);
  return distance;

}


double Sensor::getAvg()  //     *****         https://corporatefinanceinstitute.com/resources/equities/exponentially-weighted-moving-average-ewma/      ******
{
  // EMA = NewValue*(2/(_EWMA_size + 1)) + LastSimpleMovingAvg*(1-(2/(_WMA_size + 1));
  _multiplicationFactor = 0.2;      // range 0-1 the larger the more weighted towards most recent
  _EWMA = 0; 

  movingAvg = 0;
  
  if (!_avgActive) return 999999;    // Not enough values in buffer -- should only be at start, so returning BIG value works fine
  movingAvg += (_multiplicationFactor * pastElements[_arrayCursor]) + PastEWMA(_arrayCursor - 1);
  
}


double Sensor::PastEWMA(int current)        // recursive
{ 
  double prevEWMA = 0;
  if (current == _EWMA_size - 1) current = 0;   // circle back to start of array
  if (current == _arrayCursor) return 0;      // END OF RECURSION     (Back to first element)
  prevEWMA = PastEWMA(--current);
  return (_multiplicationFactor * pastElements[current]) + ((1 - _multiplicationFactor) * prevEWMA); 
}
