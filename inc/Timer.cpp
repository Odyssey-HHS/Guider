#include "Timer.h"

Timer::Timer(int stdDuration) : duration(stdDuration) {}

void Timer::start(int newDuration)
{
    if (newDuration > 0)
    {
        duration = newDuration;
    }
    start();
}

void Timer::start()
{
    timestamp = std::time(nullptr) + duration;
}

void Timer::stop()
{
    timestamp = 0;
}

bool Timer::finished()
{
    return std::time(nullptr) > timestamp;
}