#pragma once
#include "Input.h"
#include "CornException.h"
#include <vector>
#include <string>

class InputSystem;
typedef InputSystem* PInputSystem;

class InputSystem
{
	friend class Game;
public:
	class InputSysException : public CornDescriptionException
	{
	public:
		InputSysException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;
	
	static KeyCode GetFirstKeyPressCode();
	static SInput GetInput(const string& name) noexcept;
protected:
	~InputSystem() = default;
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

#define INPUTSYS_EXCEPT( description ) InputSystem::InputSysException(__LINE__,__FILE__,description)