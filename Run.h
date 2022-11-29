#pragma once

#include <time.h>


class Run
{
  private:
  long long StartTime;

  public:
  boolean DeadendReached;
  boolean DeadendTurn;
  boolean DidFinish;
  int Points;
  time_t TimeTaken;

  void startTimer(); // returns 0 if error
  void stopTimer();
  int calculatePoints();
};
