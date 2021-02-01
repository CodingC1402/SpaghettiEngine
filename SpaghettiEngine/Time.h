#pragma once
#include <Windows.h>
#include <chrono>
#include "Mathf.h"
#include "WStrUtils.h"

#define TIME_PRECISION 3
#define MAX_SYSTEM_TIME 8000 // Claculate this with pow(2, 23) / pow(10, TIME_PRECISION)
#define MAX_TIMESCALE 5

class Time
{
protected:
	static float m_fTimeScale; //0 to 1
	static float m_fDeltaTime;
	static float m_fSystemTime;
	static std::chrono::time_point<std::chrono::high_resolution_clock> m_tpPrevious;
public:
	static void UpdateTime();
	static float GetSystemTime();
	static float GetDeltaTime();
	static float GetTimeScale();

	static float SetTimeScale( float TimeScale ); // Return the time scale that has been set : ^)
};

