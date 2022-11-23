#include <SoftwareSerial.h>
SoftwareSerial HC06(10, 11); //HC06-TX Pin 10, HC06-RX to Arduino Pin 11

unsigned long prev;
void setup() {
  prev = millis();

  //Right Sensors
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);

  HC06.begin(9600); //Baudrate 9600 , Choose your own baudrate
  Serial.begin(9600);

  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}

void loop() {
  double rightSensor1 =  getDistance(2, 3);
  double rightSensor2 =  getDistance(4, 5);

  if (millis() - prev > 250)
  {
    Serial.print(rightSensor1);
    Serial.print(",");
    Serial.println(rightSensor2);

    HC06.print(rightSensor1);
    HC06.print(",");
    HC06.println(rightSensor2);
    prev = millis();
  }

}

double getDistance(int trigPin, int echoPin) {
  double duration, distance;
  digitalWrite(trigPin, LOW);
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
  }

  //delay(50);
  return distance;
}
