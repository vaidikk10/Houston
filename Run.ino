
#include "Run.h"

void Run::startTimer()
{
  StartTime = millis();
}

void Run::stopTimer()
{
  long long endTime = millis();
  TimeTaken = endTime - StartTime;
  StartTime = 0; // reset start time
}

// NOT IN CLASS
void ButtonPressed_EXTI0()
{
  buttonPressed = true;
}
