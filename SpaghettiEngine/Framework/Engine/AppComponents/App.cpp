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

BOOL App::Go()
{
	BOOL iResult = -1;

	try
	{
		timer = STimer(Timer::Create());

		auto wndSize = Setting::GetResolution();
		wnd = std::make_shared<GameWnd>(wndSize.width, wndSize.height, Setting::GetAppName());
		wnd->Show();

		Graphics::Init(Graphics::ColorFormat::RGB32Bit, wnd);
		Game::Init();

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
	Game::Update();
	Graphics::Render();
}

void App::CallQuit()
{
	__instance->running = false;
}

void App::Quit() const
{
	Game::Quit();
	__instance = nullptr;
	delete this;
}

PApp App::GetInstance() noexcept
{
	if (!__instance)
		__instance = new App();
	return __instance;
}