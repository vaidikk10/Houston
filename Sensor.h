
#ifndef SENSOR
#define SENSOR


class Sensor
{
  private:
  int trigPin;
  int echoPin;
  double reading;
  double movingAvg;
  double THRESHOLD;


  public:
  Sensor(int, int);
//  ~Sensor();
//  void add(int);
  double getReading();
  boolean isWall();
};

#endif
