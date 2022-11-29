
#include "Run.h"

void Run::startTimer()
{
  StartTime = time(0);
}

void Run::stopTimer()
{
  long long endTime = time(0);
  TimeTaken = endTime - StartTime;
  StartTime = 0; // reset start time
}
