#pragma once
#include "Timer.h"

typedef class GameTimer* PGameTimer;

class GameTimer
{
	friend class Game;
	friend class SceneManager;
public:
	static void SetTimeScale(float value);

	static float GetTimeScale();
	static float GetRealTime();
	static float GetGameTime();
	static float GetDeltaTime();
protected:
	static PGameTimer GetInstance();
	static void ResetDeltaTime();

	GameTimer();
	~GameTimer() = default;
	void Start() const;
	void Mark();
protected:
	PTimer timer = nullptr;
	float timeScale = 1;
	float deltaTime = 0;
	float realTime = 0;
	float gameTime = 0;

private:
	// If each frame exceed this number then wrong delta time is the least of your problem : |
	static constexpr float MAX_DELTA_TIME = 1;
	static PGameTimer __instance;
};