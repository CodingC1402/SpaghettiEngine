#include "App.h"
#include "json.hpp"
#include "Setting.h"
#include "Graphics.h"
#include "Game.h"
#include "GameTimer.h"
#include "SceneManager.h"
#include "Physic.h"
#include <iomanip>

PApp App::__instance = nullptr;
constexpr float _loopCap = 0.00001f;

App::~App() noexcept
{
	delete game;
	delete gfx;
	game = nullptr;
	gfx = nullptr;
}

BOOL App::Go()
{
	BOOL iResult = -1;

	try
	{
		timer = STimer(Timer::Create());
		gfx = Graphics::GetInstance();
		gfx->Init(Graphics::ColorFormat::RGB32Bit);

		game = Game::GetInstance();
		game->Init();

		wnd = gfx->GetCurrentWindow();
		wnd->Show();

		timer->Start();
		running = true;

		while ( running )
		{
			timer->Mark();
			_accumulation += timer->GetDeltaTime();
			while (_accumulation >= _loopCap)
			{
				if (Window::ProcessMessages() == WM_QUIT) {
					CallQuit();
					break;
				}
				DoFrame();
				_accumulation -= _loopCap;
			}
		}
		
		PostQuitMessage(1);
		Quit();

		iResult = 1;
	}
	catch (const CornDescriptionException& e)
	{
		MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( const CornException &e )
	{
		MessageBox( nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( const std::exception &e )
	{
		MessageBox( nullptr, StringConverter::StrToWStr(e.what()).c_str(), L"StandardException", MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( ... )
	{
		MessageBox( nullptr, L"¯\\_(ツ)_/¯ No detail ", L"UnknowException", MB_OK | MB_ICONEXCLAMATION );
	}

	return iResult;
}

void App::DoFrame() const
{
	game->Update();
	if (Physic::Update())
	{
		game->FixUpdate();
	}
	gfx->Render();
}

void App::CallQuit()
{
	__instance->running = false;
}

void App::Quit() const
{
	__instance = nullptr;
	delete this;
}

PApp App::GetInstance() noexcept
{
	if (!__instance)
		__instance = new App();
	return __instance;
}