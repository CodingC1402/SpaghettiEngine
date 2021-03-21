#pragma once
#include "Window.h"

class App
{
protected:
	Timer m_Timer;
	Window m_Window;
public:
	int Go();
	void DoFrame();
};

