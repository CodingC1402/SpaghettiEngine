#include "Time.h"

float Time::m_fTimeScale = 1; //0 to 1
float Time::m_fDeltaTime = 0;
float Time::m_fSystemTime = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_tpPrevious = std::chrono::high_resolution_clock::now();

float Time::GetSystemTime()
{
    return m_fSystemTime;
}

float Time::GetDeltaTime()
{
    return m_fDeltaTime;
}

float Time::GetTimeScale()
{
    return m_fTimeScale;
}

void Time::UpdateTime()
{
    auto tpCurrent = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> dif = tpCurrent - m_tpPrevious;

    m_fDeltaTime = Mathf::RoundToPrecision( dif.count(), TIME_PRECISION );
    m_tpPrevious = tpCurrent;

    m_fSystemTime += m_fDeltaTime;
    m_fSystemTime = Mathf::Clamp(m_fSystemTime, MAX_SYSTEM_TIME, 0);

    m_fDeltaTime *= m_fTimeScale;
}

float Time::SetTimeScale( float TimeScale )
{
    m_fTimeScale = Mathf::Clamp( TimeScale, MAX_TIMESCALE, 0 );
    return m_fTimeScale;
}
