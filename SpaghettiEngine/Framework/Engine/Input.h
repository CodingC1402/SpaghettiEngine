#pragma once
#include "KeyBoard.h"
#include "Mouse.h"
#include <memory>

class Input;
typedef Input* PInput;
typedef std::shared_ptr<Input> SInput;

using std::string;

class Input
{
	friend class InputSystem;
protected:

public:
	bool Check() const noexcept;
protected:
	Input(KeyCode keyCode, std::wstring name) noexcept;
	bool CheckName(std::wstring name) const noexcept;
	virtual void Update(KeyBoard::Event e) noexcept = 0;
	virtual void Reset() noexcept;
	void LostFocus() noexcept;
protected:
	std::wstring name;
	bool isInputActive = false;
	KeyCode keyCode = 0;
};

class InputKeyState : Input
{
	using Input::Input;
	virtual void Update(KeyBoard::Event e) noexcept override;
	virtual void Reset() noexcept override;
};

class InputKeyUp : Input
{
	using Input::Input;
	virtual void Update(KeyBoard::Event e) noexcept override;
};

class InputKeyPress : Input
{
	using Input::Input;
	virtual void Update(KeyBoard::Event e) noexcept override;
};