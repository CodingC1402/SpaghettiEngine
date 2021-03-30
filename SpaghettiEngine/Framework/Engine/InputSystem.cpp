
#include "InputSystem.h"

InputSystem* InputSystem::Create(const SWindow& wnd)
{
	return new InputSystem(wnd);
}

InputSystem::InputSystem(const SWindow& wnd)
{
	keyBoard = wnd->GetKeyBoard();
	mouse = wnd->GetMouse();
}
