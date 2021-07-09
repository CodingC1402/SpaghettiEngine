#include "Game.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "InputSystem.h"
#include "Graphics.h"
#include "Physic.h"
#include "Setting.h"
#include "Tag.h"
#include "DebugRenderer.h"

Game* Game::__instance = nullptr;

void Game::Init()
{
	timer = GameTimer::GetInstance();
	timer->Start();

	input = InputSystem::GetInstance();

	sceneManager = SceneManager::GetInstance();
	sceneManager->Init();
}

static bool IwannaDie = false;
void Game::Update() const
{
	if (InputSystem::GetInstance()->GetInput("FullScreen")->Check())
	{
		IwannaDie = !IwannaDie;
		if (IwannaDie)
			Graphics::FullScreen();
		else
			Graphics::Window();
	}

	timer->Mark();

	input->Update();
	sceneManager->Update();
	if (Physic::Update())
	{
		if constexpr (Setting::IsDebugMode())
			DebugRenderer::Clear();
		FixUpdate();
	}
}

void Game::FixUpdate() const
{
	sceneManager->FixedUpdate();
}

void Game::End()
{
	sceneManager->Unload();
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
