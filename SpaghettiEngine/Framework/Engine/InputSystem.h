#pragma once
#include "Window.h"
#include "Input.h"
#include <vector>

class InputSystem;
typedef InputSystem* PInputSystem;

class InputSystem
{
	friend class App;
public:
	KeyCode GetFirstKeyPressCode();
protected:
	static PInputSystem GetInstance();
	InputSystem();
	void Update();
protected:
	PKeyBoard kb;
	PMouse mouse;

	KeyBoard::Event firstKeyPressEvent;
	std::wstring charInput;
	std::vector<SInput> inputs;

	static PInputSystem instance;
private:
	~InputSystem() {};
};

