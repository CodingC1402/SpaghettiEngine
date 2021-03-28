#include "App.h"

void App::ChangeName()
{
	if (deltaTimeSinceLastChange >= wndChangeDeltaTime)
	{
		std::wostringstream os;
		os << wnd->GetName() << std::to_wstring(timer->GetSystemTime()).c_str() << " s";
		wnd->SetName(os.str().c_str());

		deltaTimeSinceLastChange = 0;
	}
	else
	{
		deltaTimeSinceLastChange += timer->GetDeltaTime();
	}
}

BOOL App::Go()
{
	running = true;
	wnd = new Window( 800, 600, L"spaghetti (￣▽￣)" );
	timer = new Timer();

	timer->Start();
	BOOL iResult = -1;

	try
	{
		while ( running )
		{
			timer->Mark();

			if (Window::ProcessMessages() == WM_QUIT)
			{
				this->Quit();
			}

			DoFrame();
		}

		PostQuitMessage(1);
		delete timer;
		delete wnd;

		wnd = nullptr;
		timer = nullptr;

		iResult = 1;
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
		MessageBox( nullptr, L"¯\_(ツ)_/¯ No detail ", L"UnknowException", MB_OK | MB_ICONEXCLAMATION );
	}

	return iResult;
}

void App::DoFrame()
{
	if (showInfo)
		ChangeName();
}

void App::Quit()
{
	running = false;
}

void App::ShowExtraInfo()
{
	showInfo = true;
}

void App::HideExtraInfo()
{
	showInfo = false;
	if (wnd != nullptr)
	{
		wnd->SetName(wnd->GetName());
	}
}
