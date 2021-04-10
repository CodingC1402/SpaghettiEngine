#pragma once
#include "InputSystem.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "App.h"

class App;

class Game
{
	friend class App;
public:
	void Init();
	void Update();
protected:
	Game() = default;
	~Game();

	static Game* GetInstance();
private:
	PGameTimer timer = NULL;
	PInputSystem input = NULL;
	PSceneManager sceneManager = NULL;

	static Game* __instance;
};