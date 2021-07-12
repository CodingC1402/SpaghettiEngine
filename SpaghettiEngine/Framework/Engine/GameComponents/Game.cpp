#include "Game.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "InputSystem.h"
#include "Graphics.h"
#include "Physic.h"
#include "Setting.h"
#include "Tag.h"
#include "DebugRenderer.h"

void Game::Init()
{
	timer = GameTimer::GetInstance();
	timer->Start();

	input = InputSystem::GetInstance();

	sceneManager = SceneManager::GetInstance();
	sceneManager->Init();
}

static bool IwannaDie = false;
void Game::Update()
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
	Physic::Update();
}

void Game::FixUpdate()
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