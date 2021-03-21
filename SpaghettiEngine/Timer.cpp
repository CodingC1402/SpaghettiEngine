#include "Timer.h"

double Timer::GetSystemTime()
{
    return m_SystemTime;
}

double Timer::GetDeltaTime()
{
    return m_DeltaTime;
}

double Timer::GetTimeScale()
{
    return m_TimeScale;
}

void Timer::Start()
{
    m_tpPrevious = std::chrono::high_resolution_clock::now();
}

void Timer::Mark()
{
    auto tpCurrent = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dif = tpCurrent - m_tpPrevious;

    m_DeltaTime = Mathd::RoundToPrecision( dif.count(), TIME_PRECISION );
    m_tpPrevious = tpCurrent;

    m_SystemTime += m_DeltaTime;
    m_SystemTime = Mathd::Clamp(m_SystemTime, MAX_SYSTEM_TIME, 0);

    m_DeltaTime *= m_TimeScale;
}

double Timer::SetTimeScale(double TimeScale )
{
    m_TimeScale = Mathd::Clamp( TimeScale, MAX_TIMESCALE, 0 );
    return m_TimeScale;
}
