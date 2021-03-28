#pragma once
#include <chrono>
#include "Mathd.h"
#include "CornWnd.h"

#define TIME_PRECISION 3
#define MAX_TIMESCALE 5

class Timer
{
protected:
	double deltaTime = 0;
	double systemTime = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> previousTimePoint;
public:
	void Start ();
	void Mark();
	double GetSystemTime();
	double GetDeltaTime();
};

