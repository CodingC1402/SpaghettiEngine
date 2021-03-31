#include "Input.h"

bool Input::Check() const noexcept
{
	return isInputActive;
}

Input::Input(KeyCode keyCode, std::string name)  noexcept
{
	this->keyCode = keyCode;
	this->name = name;
}

Input::Type Input::GetType() const noexcept
{
	return type;
}

bool Input::CheckName(std::string name) const noexcept
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

PInput Input::Create(KeyCode keyCode, std::string name, Type t) noexcept
{
	switch (t)
	{
	case Type::KeyState:
		return new InputKeyState(keyCode, name);
		break;
	case Type::KeyPress:
		return new InputKeyPress(keyCode, name);
		break;
	case Type::KeyUp:
		return new InputKeyUp(keyCode, name);
		break;
	default:
		return nullptr;
		break;
	}
}

InputKeyState::InputKeyState(KeyCode keyCode, std::string name) : Input(keyCode, name)
{
	type = Input::Type::KeyState;
}

void InputKeyState::Update(KeyBoard::Event e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsPress();
}

void InputKeyState::Reset() noexcept
{}

InputKeyUp::InputKeyUp(KeyCode keyCode, std::string name) : Input(keyCode, name)
{
	type = Input::Type::KeyUp;
}

void InputKeyUp::Update(KeyBoard::Event e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsRelease();
}

InputKeyPress::InputKeyPress(KeyCode keyCode, std::string name) : Input(keyCode, name)
{
	type = Input::Type::KeyPress;
}

void InputKeyPress::Update(KeyBoard::Event e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsPress();
} 
