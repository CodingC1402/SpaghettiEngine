#include "GameTimer.h"

PGameTimer GameTimer::__instance = NULL;

void GameTimer::SetTimeScale(double value)
{
	__instance->timeScale = value;
}

float GameTimer::GetTimeScale()
{
	return __instance->timeScale;
}

float GameTimer::GetRealTime()
{
	return __instance->realTime;
}

float GameTimer::GetDeltaTime()
{
	return __instance->deltaTime;
}

PGameTimer GameTimer::GetInstance()
{
	if (!__instance)
		__instance = new GameTimer();
	return __instance;
}

GameTimer::GameTimer()
{
	timer = Timer::Create();
}

void GameTimer::Start() const
{
	timer->Start();
}

void GameTimer::Mark()
{
	timer->Mark();
	realTime = timer->GetSystemTime();
	deltaTime = timer->GetDeltaTime() * timeScale;
}
