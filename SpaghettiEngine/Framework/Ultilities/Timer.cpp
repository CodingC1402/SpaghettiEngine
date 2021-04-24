#include "Timer.h"

const double& Timer::GetSystemTime() const noexcept
{
    return systemTime;
}

const double& Timer::GetDeltaTime() const noexcept
{
    return deltaTime;
}

Timer* Timer::Create() noexcept
{
    return new Timer();
}

void Timer::Start() noexcept
{
    previousTimePoint = std::chrono::high_resolution_clock::now();
}

void Timer::Mark() noexcept
{
    const auto currentTimePoint = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> dif = currentTimePoint - previousTimePoint;

    deltaTime = dif.count();
    previousTimePoint = currentTimePoint;

    systemTime += deltaTime;
    if ( systemTime < 0 )
       systemTime = 0;
}
