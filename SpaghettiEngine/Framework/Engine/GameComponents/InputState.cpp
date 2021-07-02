#include "InputState.h"
#include "KeyBoard.h"
#include "Mouse.h"

InputKeyDown::InputKeyDown(const KeyCode& keyCode, const std::string& name) noexcept : Input(keyCode, name)
{
	type = Input::Type::KeyDown;
}
void InputKeyDown::Update(const KeyBoardEvent& e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsPress();
}
void InputKeyDown::Reset() noexcept
{}

InputKeyUp::InputKeyUp(const KeyCode& keyCode, const std::string& name) noexcept : Input(keyCode, name)
{
	type = Input::Type::KeyUp;
}
void InputKeyUp::Update(const KeyBoardEvent& e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = !e.IsRelease();
}
void InputKeyUp::Reset() noexcept
{}

InputKeyPress::InputKeyPress(const KeyCode& keyCode, const std::string& name) noexcept : Input(keyCode, name)
{
	type = Input::Type::KeyPress;
}
void InputKeyPress::Update(const KeyBoardEvent& e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	if (e.IsPress() && e.IsFirstTime())
		isInputActive = true;
}

InputKeyRelease::InputKeyRelease(const KeyCode& keyCode, const std::string& name) noexcept : Input(keyCode, name)
{
	type = Type::KeyRelease;
}
void InputKeyRelease::Update(const KeyBoardEvent& e) noexcept
{
	if (e.GetCode() != keyCode)
		return;

	isInputActive = e.IsRelease();
}
