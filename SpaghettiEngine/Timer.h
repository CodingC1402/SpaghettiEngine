#pragma once
#include <Windows.h>
#include <chrono>
#include "Mathd.h"
#include "WStrUtils.h"

#define TIME_PRECISION 3
#define MAX_SYSTEM_TIME 8000 // Claculate this with pow(2, 23) / pow(10, TIME_PRECISION)
#define MAX_TIMESCALE 5

class Timer
{
protected:
	double m_TimeScale = 1; //0 to 1
	double m_DeltaTime = 0;
	double m_SystemTime = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_tpPrevious;
public:
	void Start ();
	void Mark();
	double GetSystemTime();
	double GetDeltaTime();
	double GetTimeScale();

	double SetTimeScale(double TimeScale ); // Return the time scale that has been set : ^)
};

