#include "Timer.h"

double Timer::GetSystemTime()
{
    return systemTime;
}

double Timer::GetDeltaTime()
{
    return deltaTime;
}

void Timer::Start()
{
    previousTimePoint = std::chrono::high_resolution_clock::now();
}

void Timer::Mark()
{
    auto currentTimePoint = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dif = currentTimePoint - previousTimePoint;

    deltaTime = dif.count();
    previousTimePoint = currentTimePoint;

    systemTime += deltaTime;
    if ( systemTime < 0 )
       systemTime = 0;
}
