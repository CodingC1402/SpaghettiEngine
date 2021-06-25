#pragma once

#include "KeyBoard.h"
#include "Mouse.h"
#include "json.hpp"
#include <memory>
#include <mutex>

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
		KeyRelease,
		KeyPress,
		KeyUp,
		KeyDown,
		All,
		Invalid
	};
public:
	~Input() = default;
	bool Check() noexcept;
protected:
	Input(const KeyCode& keyCode, const std::string& name) noexcept;
	Type GetType() const noexcept;
	bool CheckName(const std::string& name) const noexcept;
	virtual void Update(const KeyBoard::Event& e) noexcept = 0;
	virtual void Reset() noexcept;
	void LostFocus() noexcept;

	static SInput Create(const KeyCode& keyCode, const std::string& name, const Type& t) noexcept;
protected:
	std::recursive_mutex _inputLock;

	Type type;
	std::string name;
	bool isInputActive = false;
	KeyCode keyCode = 0;
};