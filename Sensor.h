#pragma once

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
  void Init(int, int, int);
  void add(int);
  double getReading();
  double getAvg();
  boolean isWall();
  double PastEWMA(int);
};
