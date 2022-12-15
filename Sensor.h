
#ifndef SENSOR
#define SENSOR

#include <NewPing.h>

class Sensor
{
  private:
  int trigPin;
  int echoPin;
  double reading;
  double movingAvg;
  double THRESHOLD;
  double* pastElements;
  int _EWMA_size;
  int _arrayCursor;
  boolean _avgActive;
  double _EWMA;
  double _multiplicationFactor;

  public:
  Sensor(int, int, int);
  ~Sensor();
  void add(int);
  double getReading();
  double getReading2();
  double getAvg();
  boolean isWall();
  double PastEWMA(int);

  NewPing * sonar;  // NewPing library for supposedly better sensor readings (due to some time out issue)
};

#endif
