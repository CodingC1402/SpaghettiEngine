#include "GameTimer.h"

PGameTimer GameTimer::__instance = NULL;

void GameTimer::SetTimeScale(float value)
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

float GameTimer::GetGameTime()
{
	return __instance->gameTime;
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

void GameTimer::ResetDeltaTime()
{
	GetInstance()->Mark();
	GetInstance()->deltaTime = 0;
	GetInstance()->gameTime = 0;
	GetInstance()->realTime = 0;
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
	constexpr float timeCap = 100000;

	timer->Mark();
	realTime	= timer->GetSystemTime();
	gameTime	= timer->GetSystemTime() * timeScale;
	deltaTime	= (timer->GetDeltaTime() >= MAX_DELTA_TIME ? MAX_DELTA_TIME : timer->GetDeltaTime()) * timeScale;

	if (realTime >= timeCap)
		realTime -= timeCap;
	if (gameTime >= timeCap)
		realTime -= timeCap;
}
