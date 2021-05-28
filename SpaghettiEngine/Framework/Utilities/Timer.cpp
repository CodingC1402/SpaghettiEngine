#include "Timer.h"

const float& Timer::GetSystemTime() const noexcept
{
    return systemTime;
}

const float& Timer::GetDeltaTime() const noexcept
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
    const std::chrono::duration<float> dif = currentTimePoint - previousTimePoint;

    deltaTime = dif.count();
    previousTimePoint = currentTimePoint;

    systemTime += deltaTime;
    if ( systemTime < 0 )
       systemTime = 0;
}
