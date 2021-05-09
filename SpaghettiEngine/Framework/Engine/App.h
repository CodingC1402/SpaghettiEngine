#pragma once
#include "GameWnd.h"

typedef class Graphics* PGraphics;
typedef class Game* PGame;
typedef class App* PApp;

class App
{
	friend class WinMain;
public:
	void StopRender();
	void ContinueRender();

	static void CallQuit();
	static PApp GetInstance() noexcept;
protected:
	~App() noexcept;

	BOOL Go();
	void DoFrame() const;
	void Quit() const;
protected:
	SGameWnd wnd = nullptr;
	STimer timer = nullptr;
	PGraphics gfx = nullptr;
	PGame game = nullptr;

	Size resolution;
	std::wstring appName;

	bool running = false;
	volatile bool _stopRender = false;
	
	static PApp __instance;
};