
unsigned long prev;
unsigned long T;
int buttonCount;
boolean pushed;

int GreenLED = 2;
int BlueLED = 3;
int YellowLED = 4;
int RedLED = 5;


void setup()
{
  pinMode(YellowLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  
  pinMode(A2, INPUT);
  Serial.begin(9600);

  //Initial state of button
  pushed = false;
  prev = millis(); //To track how long the button has been pressed for;
  T = millis();
  buttonCount = 0;

  
}

void loop()
{
   if(millis() - T >= 100)
   {
    T = millis();
    float voltage = analogRead(A2) * (5.0 / 1023.0);
  
    //Original State of button
    
    if(voltage >= 3 & pushed == false){
      pushed = true; //button pressed
      if(buttonCount == 0){
        prev = millis();
      }
      buttonCount++;
    }else{
      pushed = false;; //button in original state;
    }
  switch (buttonCount){
    case 0:
        turnOffLED(YellowLED);
        turnOffLED(BlueLED);
        turnOffLED(GreenLED);
        turnOffLED(RedLED);
      break;
     case 1: 
        turnOnLED(YellowLED);
  
        turnOffLED(BlueLED);
        turnOffLED(GreenLED);
        turnOffLED(RedLED);
      break;
    case 2:
        turnOnLED(BlueLED);
  
        turnOffLED(YellowLED);
        turnOffLED(GreenLED);
        turnOffLED(RedLED);
      break;
      case 3:
        turnOnLED(GreenLED);
  
        turnOffLED(YellowLED);
        turnOffLED(BlueLED);
        turnOffLED(RedLED);
      break;
      case 4:
        turnOnLED(RedLED);
  
        turnOffLED(YellowLED);
        turnOffLED(BlueLED);
        turnOffLED(GreenLED);
      break;
      case 5:
        turnOnLED(YellowLED);
        turnOnLED(BlueLED);
        turnOnLED(GreenLED);
        turnOnLED(RedLED);
        break;
      default:
        buttonCount = 0;
        break;
  }

  if(millis() - prev >= 2000000)
  {
        turnOffLED(YellowLED);
        turnOffLED(BlueLED);
        turnOffLED(GreenLED);
        turnOffLED(RedLED);
        buttonCount = 0;
        prev = millis();
  }
    Serial.print(pushed);
    Serial.print(", ");
    Serial.print(buttonCount);
    Serial.print(" --> ");
    Serial.println(millis()-prev);
   }
}

void turnOnLED(int pin){
  digitalWrite(pin, HIGH);
}

void turnOffLED(int pin){
  digitalWrite(pin, LOW);
}
