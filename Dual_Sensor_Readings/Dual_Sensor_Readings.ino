// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using HC-SR04 Module
// Tested on 17 September 2019
// ---------------------------------------------------------------- //


#define TRIG_PIN 2 //attach pin D3 Arduino to pin Trig of HC-SR04

//define echoPin1 3 // attach pin D2 Arduino to pin Echo of HC-SR04
//#define echoPin2 4 

//Sensor 1
const int trigPin1 = 2;
const int echoPin1 = 3;

//Sensor 2
const int trigPin2 = 4;
const int echoPin2 = 5;

// defines variables

void setup() {
  //Sensor 1
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin1, INPUT); // Sets the echoPin as an INPUT

  //Sensro 2
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin2, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  unsigned int sensor1 = getDistance(trigPin1, echoPin1);
  unsigned int sensor2 = getDistance(trigPin1, echoPin2);
  

  Serial.print("Sensor-1: ");
  Serial.print(sensor1);
  Serial.println(" cm");

  Serial.print("Sensor-2: ");
  Serial.print(sensor2);
  Serial.println(" cm");
  Serial.println("************");

  delay(400);
}

int getDistance(int trigPin, int echoPin){
  unsigned int duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  delay(50);
  return distance;
}
  
