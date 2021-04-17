#pragma once
#include "GameWnd.h"
#include "Physic.h"
#include "Sound.h"
#include "Game.h"

typedef class App* PApp;
typedef class Game* PGame;

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
	SGameWnd wnd = NULL;
	STimer timer = NULL;
	PGraphics gfx = NULL;
	PGame game = NULL;

	Size resolution;
	std::wstring appName;

	const wchar_t** textAnimation = nullptr;
	mutable int currentFrame = 0;
	const int numberofFrame = 17;

	mutable double fps = 0;
	mutable int framePassed = 0;
	double delayBetweenLoops = 0;

	bool running = false;

	mutable double deltaTimeSinceLastChange = 0;
	mutable double wndChangeDeltaTime = 0.1;
	mutable bool volatile showInfo = false;

	static PApp __instance;
};