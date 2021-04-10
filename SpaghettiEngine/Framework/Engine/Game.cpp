#include "Game.h"

Game* Game::__instance = NULL;

void Game::Init()
{
	timer = GameTimer::GetInstance();
	timer->Start();
	sceneManager = SceneManager::GetInstance();
	sceneManager->Init();
	input = InputSystem::GetInstance();
}

void Game::Update()
{
	timer->Mark();
	input->Update();

	if (InputSystem::GetInput("Left")->Check())
	{
		App::CallQuit();
	}
	if (InputSystem::GetInput("Up")->Check())
	{
		Graphics::ToFullScreenMode();
	}
	if (InputSystem::GetInput("Down")->Check())
	{
		Graphics::ToWindowMode();
	}

	sceneManager->Update();
}

Game::~Game()
{
	if (input)
		delete input;
	if (timer)
		delete timer;

	input = NULL;
	timer = NULL;
}

Game* Game::GetInstance()
{
	if (!__instance)
		__instance = new Game();
	return __instance;
}
