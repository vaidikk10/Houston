
#pragma once

class Sensor
{
  private:
  int trigPin;
  int echoPin;
  double reading;
  double movingAvg;
  double THRESHOLD;

  public:
  Sensor(int, int);    // constructor
  void add(int);
  double getReading();
  double getAvg();
  boolean isWall();
};
