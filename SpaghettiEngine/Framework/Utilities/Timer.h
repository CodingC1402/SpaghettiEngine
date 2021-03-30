#pragma once
#include <chrono>
#include "CornWnd.h"
#include <memory>

#define TIME_PRECISION 3
#define MAX_TIMESCALE 5

typedef class Timer
{
public:
	~Timer() {};
	void Start() noexcept;
	void Mark() noexcept;
	const double& GetSystemTime() noexcept;
	const double& GetDeltaTime() noexcept;

	void Destroy() noexcept;
	static Timer* Create() noexcept;
protected:
	Timer() {};
protected:
	double deltaTime = 0;
	double systemTime = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> previousTimePoint;
} *PTimer;

typedef std::unique_ptr<Timer> UTimer;
typedef std::shared_ptr<Timer> STimer;

