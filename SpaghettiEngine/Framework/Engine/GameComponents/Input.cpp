#include "Input.h"
#include "InputState.h"
#include "InputAll.h"

bool Input::Check() noexcept
{
	std::lock_guard guard(_inputLock);
	return isInputActive;
}

Input::Input(const KeyCode& keyCode, const std::string& name)  noexcept
{
	this->keyCode = keyCode;
	this->name = name;
}

Input::Type Input::GetType() const noexcept
{
	return type;
}

bool Input::CheckName(const std::string& name) const noexcept
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

SInput Input::Create(const KeyCode& keyCode, const std::string& name, const Type& t) noexcept
{
	switch (t)
	{
	case Type::KeyUp:
		return std::shared_ptr<InputKeyUp>(new InputKeyUp(keyCode, name));
		break;
	case Type::KeyDown:
		return std::shared_ptr<InputKeyDown>(new InputKeyDown(keyCode, name));
		break;
	case Type::KeyPress:
		return std::shared_ptr<InputKeyPress>(new InputKeyPress(keyCode, name));
		break;
	case Type::KeyRelease:
		return std::shared_ptr<InputKeyRelease>(new InputKeyRelease(keyCode, name));
		break;
	[[likely]] case Type::All:
		return std::shared_ptr<InputAll>(new InputAll(keyCode, name));
		break;
	default:
		return SInput();
	}
}