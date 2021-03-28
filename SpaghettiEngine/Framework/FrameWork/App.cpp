#include "App.h"

void App::ChangeName()
{
	if (deltaTimeSinceLastChange >= wndChangeDeltaTime)
	{
		std::wostringstream os;
		os << wnd->GetName() << ' ' << std::to_wstring(timer->GetSystemTime()).c_str() << " s";
		os << textAnimation[currentFrame];
		currentFrame++;
		currentFrame %= numberofFrame;
		wnd->SetName(os.str().c_str());

		deltaTimeSinceLastChange = 0;
	}
	else
	{
		deltaTimeSinceLastChange += timer->GetDeltaTime();
	}
}

App::App()
{
	textAnimation = new const wchar_t*[numberofFrame];
	textAnimation[0] = L" (ﾒ￣‿￣)︻┻┳══━一				  ";
	textAnimation[1] = L"(ﾒ￣▽￣)︻┻┳══━一 - ☆			  ";
	textAnimation[2] = L"(ﾒ￣▽￣)︻┻┳══━一-- - ☆		  ";
	textAnimation[3] = L"(ﾒ￣▽￣)︻┻┳══━一---- - ☆		  ";
	textAnimation[4] = L" (ﾒ￣▽￣)︻┻┳══━一------☆		  ";
	textAnimation[5] = L" (ﾒ￣▽￣)︻┻┳══━一--------☆	  ";
	textAnimation[6] = L" (ﾒ￣▽￣)︻┻┳══━一・・--------☆ ";
	textAnimation[7] = L" (ﾒ￣▽￣)︻┻┳══━一 ・・・--------";
	textAnimation[8] = L" (ﾒ￣▽￣)︻┻┳══━一   ・・・------";
	textAnimation[9] = L" (ﾒ￣▽￣)︻┻┳══━一     ・・・----";
	textAnimation[10] = L" (ﾒ￣▽￣)︻┻┳══━一       ・・・--";
	textAnimation[11] = L" (ﾒ￣▽￣)︻┻┳══━一         ・・・";
	textAnimation[12] = L" (ﾒ￣▽￣)︻┻┳══━一           ・  ";
	textAnimation[13] = L" (ﾒ￣‿￣)︻┻┳══━一				   ";
	textAnimation[14] = L" (ﾒ￣‿￣)︻┻┳══━一				   ";
	textAnimation[15] = L" (ﾒ￣‿￣)︻┻┳══━一				   ";
	textAnimation[16] = L" (ﾒ￣‿￣)︻┻┳══━一				   ";
	textAnimation[17] = L" (ﾒ￣‿￣)︻┻┳══━一				   ";
	textAnimation[18] = L" (ﾒ￣‿￣)︻┻┳══━一				   ";
	textAnimation[19] = L" (ﾒ￣‿￣)︻┻┳══━一				   ";
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
