#ifndef SENSOR
#define SENSOR


class Sensor
{
  private:
  int trigPin;
  int echoPin;
  double reading;
  double THRESHOLD;

  public:
  Sensor(int, int);
//  ~Sensor();
  double getReading();
  double getAvg();  
};

#endif
