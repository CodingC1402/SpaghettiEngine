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

	static SInput GetInput(const string& name) noexcept;
protected:
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	~InputSystem() {};
	InputSystem();

	void Update();

	void Load();
	void Save();

	static PInputSystem GetInstance();
protected:
	PKeyBoard kb;
	PMouse mouse;

	KeyBoard::Event firstKeyPressEvent;
	std::wstring charInput;
	std::vector<SInput> inputs;

	static PInputSystem __instance;
};

