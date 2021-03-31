#include "InputSystem.h"
#include "Debug.h"

PInputSystem InputSystem::instance = nullptr;

KeyCode InputSystem::GetFirstKeyPressCode()
{
	return KeyCode();
}

PInputSystem InputSystem::GetInstance()
{
	if (!instance)
		instance = new InputSystem();
	return instance;
}

InputSystem::InputSystem()
{
	kb = KeyBoard::GetInstance();
	mouse = Mouse::GetInstance();
}

void InputSystem::Update()
{
	charInput = L"";
	for (int i = 0; i < inputs.size(); i++)
	{
		inputs[i]->Reset();
	}

	KeyBoard::Event e;
	while (!kb->IsKeyEmpty())
	{
		e = kb->ReadKey();
		if (!e.IsValid())
		{
			for (int i = 0; i < inputs.size(); i++)
			{
				inputs[i]->LostFocus();
			}
		}
		else
		{
			for (int i = 0; i < inputs.size(); i++)
			{
				inputs[i]->Update(e);
			}
		}
		charInput += kb->ReadChar();
	}
}
