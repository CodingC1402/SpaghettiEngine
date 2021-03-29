#include "Timer.h"

const double& Timer::GetSystemTime() noexcept
{
    return systemTime;
}

const double& Timer::GetDeltaTime() noexcept
{
    return deltaTime;
}

void Timer::Start() noexcept
{
    previousTimePoint = std::chrono::high_resolution_clock::now();
}

void Timer::Mark() noexcept
{
    auto currentTimePoint = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dif = currentTimePoint - previousTimePoint;

    deltaTime = dif.count();
    previousTimePoint = currentTimePoint;

    systemTime += deltaTime;
    if ( systemTime < 0 )
       systemTime = 0;
}
