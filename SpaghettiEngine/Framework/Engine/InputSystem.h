#pragma once
#include "..\Window\Window.h"

class InputSystem
{
public:
	KeyBoard *kb;
public:
	InputSystem* CreateInstance(KeyBoard& kb)
	{
		this->kb = &kb;
	}
};