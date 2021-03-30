#pragma once
#include "Window.h"
#include "Graphics.h"
#include "InputSystem.h"
#include "Physic.h"
#include "Sound.h"

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
	SWindow wnd = nullptr;
	STimer timer = nullptr;
	SInputSystem input = nullptr;
	Graphics* gfx = nullptr;
	Physic* pe = nullptr;

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