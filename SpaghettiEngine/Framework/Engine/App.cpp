﻿#include "App.h"
#include <iomanip>

void App::ChangeName() const noexcept
{
	if (deltaTimeSinceLastChange >= wndChangeDeltaTime)
	{
		CalculateFPS();

		std::wostringstream os;

		os << wnd->GetName();
		os << " | FPS: " << std::left << std::setfill(L' ') << std::setw(6) << std::setprecision(5);
		if (fps < 2000)
		{
			os << fps;
		}
		else
		{
			os << "2000+";
		}
		os << L" | " << textAnimation[currentFrame];
		currentFrame++;
		currentFrame %= numberofFrame;
		wnd->SetTempName(os.str().c_str());

		deltaTimeSinceLastChange = 0;
	}
	else
	{
		deltaTimeSinceLastChange += timer->GetDeltaTime();
	}
}

void App::CalculateFPS() const noexcept
{
	fps = fps * 0.8 + (framePassed / deltaTimeSinceLastChange) * (0.2);
	framePassed = 0;
}

App::App() noexcept
{
	this->timer = nullptr;
	this->wnd = nullptr;
	this->textAnimation = nullptr;

	textAnimation = new const wchar_t*[numberofFrame];
	textAnimation[0]  = L"  (ﾒ￣‿￣)︻┻┳══━一					";
	textAnimation[1]  = L"(ﾒ￣‿￣)︻┻┳══━一---☆					";
	textAnimation[2]  = L"(ﾒ￣▽￣)︻┻┳══━一-----☆				";
	textAnimation[3]  = L"(ﾒ￣▽￣)︻┻┳══━一-------☆				";
	textAnimation[4]  = L" (ﾒ￣▽￣)︻┻┳══━一--------☆			";
	textAnimation[5]  = L"  (ﾒ￣▽￣)︻┻┳══━一---------☆			";
	textAnimation[6]  = L"  (ﾒ￣▽￣)︻┻┳══━一・・-------☆		";
	textAnimation[7]  = L"  (ﾒ￣▽￣)︻┻┳══━一 ・・・-------☆	";
	textAnimation[8]  = L"  (ﾒ￣▽￣)︻┻┳══━一   ・・・------		";
	textAnimation[9]  = L"  (ﾒ￣▽￣)︻┻┳══━一     ・・・----		";
	textAnimation[10] = L"  (ﾒ￣▽￣)︻┻┳══━一       ・・・--		";
	textAnimation[11] = L"  (ﾒ￣▽￣)︻┻┳══━一         ・・・		";
	textAnimation[12] = L"  (ﾒ￣▽￣)︻┻┳══━一           ・・		";
	textAnimation[13] = L"  (ﾒ￣‿￣)︻┻┳══━一             ・		";
	textAnimation[14] = L"  (ﾒ￣‿￣)︻┻┳══━一					";
	textAnimation[15] = L"  (ﾒ￣‿￣)︻┻┳══━一					";
	textAnimation[16] = L"  (ﾒ￣‿￣)︻┻┳══━一					";
}

App::~App()
{
	delete[] textAnimation;
}

BOOL App::Go()
{
	running = true;
	wnd = Window::GetInstance();
	wnd->SetName(L"Spaghetti Engine");
	
	input = InputSystem::GetInstance();

	timer = STimer(Timer::Create());

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

		wnd->SetName(L""); 
		PostQuitMessage(1);

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
	framePassed++;
	if (showInfo)
		ChangeName();

	input->Update();


}

void App::Quit()
{
	running = false;
}

void App::ShowExtraInfo() const noexcept
{
	showInfo = true;
}

void App::HideExtraInfo() const noexcept
{
	showInfo = false;
	if (wnd != nullptr)
	{
		wnd->SetName(wnd->GetName());
	}
}
