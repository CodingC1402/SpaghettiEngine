#include "Input.h"

bool Input::Check() const noexcept
{
	return isInputActive;
}

Input::Input(KeyCode keyCode, std::wstring name)  noexcept
{
	this->keyCode = keyCode;
	this->name = name;
}

bool Input::CheckName(std::wstring name) const noexcept
{
	return name == this->name;
}

void Input::Reset() noexcept
{
	isInputActive = false;
}

void Input::LostFocus() noexcept
{
	isInputActive = false;
}

void InputKeyState::Update(KeyBoard::Event e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsPress();
}

void InputKeyState::Reset() noexcept
{}

void InputKeyUp::Update(KeyBoard::Event e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsRelease();
}

void InputKeyPress::Update(KeyBoard::Event e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsPress();
}
