#pragma once
#include "../Window/Window.h"

class App
{
protected:
	void ChangeName();
public:
	App();

	BOOL Go();
	void DoFrame();
	void Quit();

	void ShowExtraInfo();
	void HideExtraInfo();
protected:
	Window* wnd;
	Timer* timer;

	const wchar_t** textAnimation;
	int currentFrame = 0;
	int numberofFrame = 20;

	bool running;

	double deltaTimeSinceLastChange = 0;
	double wndChangeDeltaTime = 0.1;
	bool showInfo = false;
};