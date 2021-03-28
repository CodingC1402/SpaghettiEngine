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
	Window* wnd = nullptr;
	Timer* timer = nullptr;

	const wchar_t** textAnimation = nullptr;
	int currentFrame = 0;
	int numberofFrame = 17;

	bool running = false;

	double deltaTimeSinceLastChange = 0;
	double wndChangeDeltaTime = 0.1;
	bool showInfo = false;
};