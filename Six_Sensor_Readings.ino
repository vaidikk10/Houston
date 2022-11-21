// ---------------------------------------------------------------- //
// Testing 6 HC-SR04 Sensors
// Reads all 6 sensor readings (~5ms for each sensor readin)
// Reads 10 times and then prints all the distances
// Could be adapated when calculating 10-Point Moving average
// ---------------------------------------------------------------- //
unsigned long prev;
int count;
void setup() {
  prev = millis();
  //Right Sensor Pins
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  
  //Back Sensor Pins
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  
  //Front Sensor Pins
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
  
  //Left Sensor Pins
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  
  double rightSensor1 = getDistance(2, 3);
  double frontSensor = getDistance(8,9);
  double rightSensor2 = getDistance(4, 5);
  double leftSensor1 = getDistance(10, 11);
  double backSensor = getDistance(6,7);
  double leftSensor2 = getDistance(12, 13);
  count++;
  
  if(millis()-prev > 1000)
  {
  Serial.print("Right\t: ");
  Serial.print(rightSensor1);
  Serial.print("cm, ");
  Serial.print(rightSensor2);

  Serial.print("\nLeft\t: ");
  Serial.print(leftSensor1);
  Serial.print("cm, ");
  Serial.print(leftSensor2);
  Serial.print("cm");

  Serial.print("\nFront\t: ");
  Serial.print(frontSensor);
  Serial.print("cm");

  Serial.print("\nBack\t: ");
  Serial.print(backSensor);
  Serial.print("cm\n");

  Serial.println(count); 
  Serial.println("********************");
  prev = millis();
  count = 0;
  }
}

/**
 * param: trigPin, echoPin
 * takes about ~1.2 ms when called i.e. each sensor reading is 1.2 ms
 * returns : distance or -1 if sensor is disconnectd
 */
double getDistance(int trigPin, int echoPin){
  double duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  if(duration ==0){
    Serial.print("Sensor Disconnected [Trig, Echo] : ");
    Serial.print(trigPin);
    Serial.print(", ");
    Serial.println(echoPin);
    return -1;
  }else{
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  }
  
  //delay(50);
  return distance;
}
  
