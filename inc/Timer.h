#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class Timer
{
public:
  Timer(int);
  void start(int);
  void start();
  void stop();
  bool finished();

private:
  int duration;
  std::time_t timestamp = 0;
};
#endif // TIMER_H