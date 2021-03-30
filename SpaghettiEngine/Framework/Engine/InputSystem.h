#pragma once
#include "Window.h"

class InputSystem;
typedef std::shared_ptr<InputSystem> SInputSystem;
typedef std::unique_ptr<InputSystem> UInputSystem;

class InputSystem
{
	friend class App;
protected:
	SKeyBoard keyBoard;
	SMouse mouse;
public:

protected:
	static InputSystem* Create(const SWindow& wnd);
	InputSystem(const SWindow& wnd);
};

