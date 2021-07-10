#pragma once
//
// Contain all the input that is specific to exactly one event
//
#include "Input.h"

class InputKeyDown : public Input
{
	friend class InputSystem;
	friend class Input;
protected:
	InputKeyDown(const KeyCode& keyCode, const std::string& name) noexcept;
	virtual void Update(const KeyBoardEvent& e) noexcept override;
	virtual void Reset() noexcept override;
};

class InputKeyUp : public Input
{
	friend class InputSystem;
	friend class Input;
protected:
	InputKeyUp(const KeyCode& keyCode, const std::string& name) noexcept;
	using Input::Input;
	virtual void Update(const KeyBoardEvent& e) noexcept override;
	virtual void Reset() noexcept override;
};

class InputKeyPress : public Input
{
	friend class InputSystem;
	friend class Input;
protected:
	InputKeyPress(const KeyCode& keyCode, const std::string& name) noexcept;
	using Input::Input;
	virtual void Update(const KeyBoardEvent& e) noexcept override;
protected:
	bool isFirstTime = true;
};

class InputKeyRelease : public Input
{
	friend class InputSystem;
	friend class Input;
protected:
	InputKeyRelease(const KeyCode& keyCode, const std::string& name) noexcept;
	using Input::Input;
	virtual void Update(const KeyBoardEvent& e) noexcept override;
protected:
	bool isFirstTime = true;
};