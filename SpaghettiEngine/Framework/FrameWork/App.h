#pragma once
#include "../Window/Window.h"

class App
{
protected:
	void ChangeName();
public:
	BOOL Go();
	void DoFrame();
	void Quit();

	void ShowExtraInfo();
	void HideExtraInfo();
protected:
	Window *wnd;
	Timer *timer;

	bool running;

	double deltaTimeSinceLastChange = 0;
	double wndChangeDeltaTime = 0.1;
	bool showInfo = false;
};