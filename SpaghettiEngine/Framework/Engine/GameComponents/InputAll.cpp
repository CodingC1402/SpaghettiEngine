#include "InputAll.h"
#include "KeyBoard.h"
#include "Mouse.h"

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

InputAll::InputAll(const int& keyCode, const std::string& name) noexcept : Input(keyCode, name)
{
	type = Type::All;
}

void InputAll::Update(const KeyBoardEvent& e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	bool isRelease = e.IsRelease();
	bool isPress = e.IsPress() && e.IsFirstTime();

	if (isRelease)
	{
		_isKeyDown = false;
		_isKeyRelease = true;
	}
	else if (isPress)
	{
		_isKeyDown = true;
		_isKeyPress = true;
	}

	isInputActive = isRelease || isPress;
}

void InputAll::Reset() noexcept
{
	isInputActive = false;
	_isKeyPress = false;
	_isKeyRelease = false;
}

