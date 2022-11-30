#pragma once



class Run
{
  private:
  long long StartTime;

  public:
  boolean DeadendReached;
  boolean DeadendTurn;
  boolean DidFinish;
  int Points;
  unsigned long TimeTaken;

  void startTimer(); // returns 0 if error
  void stopTimer();
  int calculatePoints();
};
