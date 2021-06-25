#include "InputAll.h"

bool InputAll::CheckKeyUp() const noexcept
{
	return !_isKeyDown;
}

bool InputAll::CheckKeyDown() const noexcept
{
	return _isKeyDown;
}

bool InputAll::CheckKeyPress() const noexcept
{
	return _isKeyPress;
}

bool InputAll::CheckKeyRelease() const noexcept
{
	return _isKeyRelease;
}

InputAll::InputAll(const KeyCode& keyCode, const std::string& name) noexcept : Input(keyCode, name)
{
	type = Type::All;
}

void InputAll::Update(const KeyBoard::Event& e) noexcept
{
	if (e.GetCode() != keyCode || !e.IsFirstTime())
		return;


	if (e.IsRelease() && _isKeyDown)
	{
		_isKeyDown = false;
		_isKeyRelease = true;
	}
	else if (e.IsPress() && !_isKeyDown)
	{
		_isKeyDown = true;
		_isKeyPress = true;
	}
}

void InputAll::Reset() noexcept
{
	isInputActive = false;
	_isKeyPress = false;
	_isKeyRelease = false;
}

