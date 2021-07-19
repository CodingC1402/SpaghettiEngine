#include "Game.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "InputSystem.h"
#include "Graphics.h"
#include "Physic.h"
#include "Setting.h"
#include "Tag.h"
#include "DebugRenderer.h"
#include "App.h"

constexpr auto fullScreenKey = "FullScreen";
constexpr auto previousScreenKey = "PreviousScreen";
constexpr auto nextScreenKey = "NextScreen";

void Game::CallQuit()
{
	App::GetInstance()->CallQuit();
}

void Game::Init()
{
	InputSystem::GetInstance()->CreateInput(Input::Type::KeyPress, fullScreenKey, 0x7A); // F11
	InputSystem::GetInstance()->CreateInput(Input::Type::KeyPress, nextScreenKey, 0xBE); // >
	InputSystem::GetInstance()->CreateInput(Input::Type::KeyPress, previousScreenKey, 0xBC); // <

	timer = GameTimer::GetInstance();
	timer->Start();
	//GameTimer::SetTimeScale(0.1f);

	input = InputSystem::GetInstance();

	sceneManager = SceneManager::GetInstance();
	sceneManager->Init();
}

static bool IwannaDie = false;
void Game::Update()
{
	if (InputSystem::GetInstance()->GetInput(fullScreenKey)->Check())
	{
		IwannaDie = !IwannaDie;
		if (IwannaDie)
			Graphics::FullScreen();
		else
			Graphics::Window();
	}
	if (InputSystem::GetInstance()->GetInput(previousScreenKey)->Check())
	{
		sceneManager->CallLoadPreviousScene();
	}
	if (InputSystem::GetInstance()->GetInput(nextScreenKey)->Check())
	{
		sceneManager->CallLoadNextScene();
	}

	timer->Mark();

	input->Update();
	sceneManager->Update();
	Physic::Update();
	sceneManager->LateUpdate();
	sceneManager->CleanUp();
}

void Game::Quit()
{
	sceneManager->Unload();
	App::GetInstance()->CallQuit();
}

void Game::FixUpdate()
{
	sceneManager->FixedUpdate();
}

void Game::ResetGameTimer() noexcept
{
	GameTimer::ResetDeltaTime();
	Physic::Reset();
}

Game::~Game()
{
	delete input;
	delete timer;
	input = nullptr;
	timer = nullptr;
}