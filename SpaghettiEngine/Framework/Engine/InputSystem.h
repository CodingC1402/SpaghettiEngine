#pragma once
#include "Window.h"
#include "Input.h"
#include "json.hpp"
#include <vector>
#include <fstream>
#include <string>

#define INPUTPATH "Input.json"
#define INPUTTEXT "Input"
#define SIZETEXT "Size"
#define TYPETEXT "Type"
#define NAMETEXT "Name"
#define CODETEXT "KeyCode"

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

	void Load();
	void Save();
protected:
	PKeyBoard kb;
	PMouse mouse;

	KeyBoard::Event firstKeyPressEvent;
	std::wstring charInput;
	std::vector<SInput> inputs;

	static PInputSystem instance;
};

