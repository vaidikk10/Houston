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


// NOT IN CLASS 
void ButtonPressed_EXTI0(void);  // Pin 2 for external interrupt 0
volatile boolean buttonPressed; // make sure this initializes as false
