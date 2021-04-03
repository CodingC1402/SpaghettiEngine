#pragma once
#include "GameWnd.h"
#include "Graphics.h"
#include "InputSystem.h"
#include "Physic.h"
#include "Game.h"
#include "Sound.h"

typedef class App* PApp;

class App
{
	friend int WINAPI wWinMain(
		HINSTANCE	hInstance,
		HINSTANCE	hPrevInstance,
		PWSTR		pCmdLine,
		int			nCmdShow
	);
public:
	static void CallQuit();
protected:
	void ChangeName() const noexcept;
	void CalculateFPS() const noexcept;

	App() noexcept;
	~App() noexcept;

	BOOL Go();
	void DoFrame();
	void Quit();

	void ShowExtraInfo() const noexcept;
	void HideExtraInfo() const noexcept;

	static PApp GetInstance() noexcept;
protected:
	SGameWnd wnd = nullptr;
	PInputSystem input = nullptr;
	PGraphics gfx = nullptr;
	STimer timer = nullptr;

	Physic* pe = nullptr;

	Size resolution;
	std::wstring appName;

	const wchar_t** textAnimation = nullptr;
	mutable int currentFrame = 0;
	const int numberofFrame = 17;

	mutable double fps = 0;
	mutable int framePassed = 0;

	bool running = false;

	mutable double deltaTimeSinceLastChange = 0;
	mutable double wndChangeDeltaTime = 0.1;
	mutable bool volatile showInfo = false;

	static PApp __instance;
};