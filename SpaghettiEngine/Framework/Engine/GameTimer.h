#pragma once
#include "Timer.h"

typedef class GameTimer* PGameTimer;

class GameTimer
{
	friend class Game;
public:
	static void SetTimeScale(double value);

	static double GetTimeScale();
	static double GetRealTime();
	static double GetDeltaTime();
protected:
	static PGameTimer GetInstance();
	
	GameTimer();
	~GameTimer() = default;
	void Start();
	void Mark();
protected:
	PTimer timer = NULL;
	double timeScale = 1;
	double deltaTime = 0;
	double realTime = 0;

	static PGameTimer __instance;
};