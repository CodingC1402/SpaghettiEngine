#pragma once

#include "KeyBoard.h"
#include "Mouse.h"
#include "json.hpp"
#include <memory>

class Input;
typedef Input* PInput;
typedef std::shared_ptr<Input> SInput;

using std::string;

class Input
{
	friend class InputSystem;
public:
	enum class Type
	{
		KeyState,
		KeyPress,
		KeyUp,
		Invalid
	};
public:
	~Input() = default;
	bool Check() const noexcept;
protected:
	Input(const KeyCode& keyCode, const std::string& name) noexcept;
	Type GetType() const noexcept;
	bool CheckName(const std::string& name) const noexcept;
	virtual void Update(const KeyBoard::Event& e) noexcept = 0;
	virtual void Reset() noexcept;
	void LostFocus() noexcept;

	static PInput Create(const KeyCode& keyCode, const std::string& name, const Type& t) noexcept;
protected:
	Type type;
	std::string name;
	bool isInputActive = false;
	KeyCode keyCode = 0;
};

class InputKeyState : public Input
{
	friend class InputSystem;
	friend class Input;
protected:
	InputKeyState(const KeyCode& keyCode, const std::string& name) noexcept;
	virtual void Update(const KeyBoard::Event& e) noexcept override;
	virtual void Reset() noexcept override;
};

class InputKeyUp : public Input
{
	friend class InputSystem;
	friend class Input;
protected:
	InputKeyUp(const KeyCode& keyCode, const std::string& name) noexcept;
	using Input::Input;
	virtual void Update(const KeyBoard::Event& e) noexcept override;
};

class InputKeyPress : public Input
{
	friend class InputSystem;
	friend class Input;
protected:
	InputKeyPress(const KeyCode& keyCode, const std::string& name) noexcept;
	using Input::Input;
	virtual void Update(const KeyBoard::Event& e) noexcept override;
};