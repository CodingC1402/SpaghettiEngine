#pragma once
#include "Input.h"

class InputAll : public Input
{
	friend class InputSystem;
	friend class Input;
public:
	bool CheckKeyUp() const noexcept;
	bool CheckKeyDown() const noexcept;

	bool CheckKeyPress() const noexcept;
	bool CheckKeyRelease() const noexcept;

	// Check now will be a function that return whether this button have been interacted with;
protected:
	InputAll(const KeyCode& keyCode, const std::string& name) noexcept;
	virtual void Update(const KeyBoard::Event& e) noexcept override;
	virtual void Reset() noexcept override;
protected:
	bool _isKeyDown = false;
	bool _isKeyPress = false;
	bool _isKeyRelease = false;
};