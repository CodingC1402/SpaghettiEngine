#include "Game.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "InputSystem.h"
#include "Graphics.h"
#include "Physic.h"

Game* Game::__instance = nullptr;

void Game::Init()
{
	timer = GameTimer::GetInstance();
	timer->Start();
	input = InputSystem::GetInstance();
	sceneManager = SceneManager::GetInstance();
	sceneManager->Init();
	physic = Physic::GetInstance();
}

void Game::Update() const
{
	timer->Mark();
	input->Update();

	if (InputSystem::GetInput("Left")->Check())
	{
		SceneManager::CallLoadPreviousScene();
	}
	if (InputSystem::GetInput("Up")->Check())
	{
		Graphics::ToFullScreenMode();
	}
	if (InputSystem::GetInput("Down")->Check())
	{
		Graphics::ToWindowMode();
	}
	if (InputSystem::GetInput("Right")->Check())
	{
		SceneManager::CallLoadNextScene();
	}

	sceneManager->Update();
	physic->Update();
}

Game::~Game()
{
	delete input;
	delete timer;
	input = nullptr;
	timer = nullptr;
}

Game* Game::GetInstance()
{
	if (!__instance)
		__instance = new Game();
	return __instance;
}
