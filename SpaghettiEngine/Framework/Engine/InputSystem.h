#pragma once
#include "Window.h"
#include "Input.h"
#include <vector>
#include <string>

class InputSystem;
typedef InputSystem* PInputSystem;

class InputSystem
{
	friend class Game;
public:
	class InputSysException : public CornDiscriptionException
	{
	public:
		InputSysException(int line, const char* file, std::wstring discription) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
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

#define INPUTSYS_EXCEPT( discription ) InputSystem::InputSysException(__LINE__,__FILE__,discription);