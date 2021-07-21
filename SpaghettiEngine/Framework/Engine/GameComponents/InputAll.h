#pragma once
#include "Input.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(InputAll);

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
	InputAll(const int& keyCode, const std::string& name) noexcept;
	virtual void Update(const KeyBoardEvent& e) noexcept override;
	virtual void Reset() noexcept override;
protected:
	bool _isKeyDown = false;
	bool _isKeyPress = false;
	bool _isKeyRelease = false;
};