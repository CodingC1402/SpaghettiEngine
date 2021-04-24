#pragma once
#include "GameWnd.h"

typedef class Graphics* PGraphics;
typedef class Game* PGame;
typedef class App* PApp;

class App
{
	friend class WinMain;
public:
	static void CallQuit();
protected:
	~App() noexcept;

	BOOL Go();
	void DoFrame() const;
	void Quit() const;

	static PApp GetInstance() noexcept;
protected:
	SGameWnd wnd = nullptr;
	STimer timer = nullptr;
	PGraphics gfx = nullptr;
	PGame game = nullptr;

	Size resolution;
	std::wstring appName;

	bool running = false;
	
	static PApp __instance;
};