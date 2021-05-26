#pragma once
#include "Timer.h"

typedef class GameTimer* PGameTimer;

class GameTimer
{
	friend class Game;
public:
	static void SetTimeScale(double value);

	static float GetTimeScale();
	static float GetRealTime();
	static float GetDeltaTime();
protected:
	static PGameTimer GetInstance();
	
	GameTimer();
	~GameTimer() = default;
	void Start() const;
	void Mark();
protected:
	PTimer timer = nullptr;
	float timeScale = 1;
	float deltaTime = 0;
	float realTime = 0;

	static PGameTimer __instance;
};