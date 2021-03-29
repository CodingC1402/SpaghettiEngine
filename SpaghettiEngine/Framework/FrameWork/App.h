#pragma once
#include "../Window/Window.h"

class App
{
protected:
	void ChangeName() const noexcept;
	void CalculateFPS() const noexcept;
public:
	App() noexcept;
	~App();

	BOOL Go();
	void DoFrame();
	void Quit();

	void ShowExtraInfo() const noexcept;
	void HideExtraInfo() const noexcept;
protected:
	Window* wnd = nullptr;
	Timer* timer = nullptr;

	const wchar_t** textAnimation = nullptr;
	mutable int currentFrame = 0;
	const int numberofFrame = 17;

	mutable double fps = 0;
	mutable int framePassed = 0;

	bool running = false;

	mutable double deltaTimeSinceLastChange = 0;
	mutable double wndChangeDeltaTime = 0.1;
	mutable bool volatile showInfo = false;
};