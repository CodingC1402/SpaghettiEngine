#include "InputSystem.h"
#include "Debug.h"
#include "json.hpp"
#include "StringConverter.h"
#include "SpaghettiEnginePath.h"
#include <sstream>
#include <fstream>

#define INPUTTEXT "Inputs"
#define TYPETEXT "Type"
#define NAMETEXT "Name"
#define CODETEXT "KeyCode"

PInputSystem InputSystem::__instance = nullptr;

KeyCode InputSystem::GetFirstKeyPressCode()
{
	return KeyCode();
}

SInput InputSystem::GetInput(const string& name) noexcept
{
	for (int i = 0; i < __instance->inputs.size(); i++)
	{
		if (__instance->inputs[i]->CheckName(name))
		{
			return __instance->inputs[i];
		}
	}

	return SInput();
}

PInputSystem InputSystem::GetInstance()
{
	if (!__instance)
		__instance = new InputSystem();
	return __instance;
}

InputSystem::InputSystem()
{
	kb = KeyBoard::GetInstance();
	mouse = Mouse::GetInstance();

	Load();
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

void InputSystem::Load()
{
	using namespace nlohmann;

	std::ifstream jsonStream(INPUTPATH);
	if (!jsonStream.is_open())
	{
		std::wostringstream oss;
		oss << L"File ";
		oss << INPUTPATH;
		oss << L" doesn't exist";
		throw INPUTSYS_EXCEPT(oss.str().c_str());
	}
	json file;
	try
	{
		jsonStream >> file;
		for (const auto& input : file[INPUTTEXT])
		{
			Input::Type type = input[TYPETEXT].get<Input::Type>();
			std::string name = input[NAMETEXT].get<std::string>();
			KeyCode		code = input[CODETEXT].get<KeyCode>();
			inputs.push_back(SInput(Input::Create(code, name, type)));
		}
	}
	catch (...)
	{
		std::wostringstream oss;
		oss << L"File ";
		oss << INPUTPATH;
		oss << L" is in the wrong format";
		throw INPUTSYS_EXCEPT(oss.str());
	}
}

NLOHMANN_JSON_SERIALIZE_ENUM(Input::Type, {
	{Input::Type::KeyPress, "KeyPress"},
	{Input::Type::KeyUp, "KeyUp"},
	{Input::Type::KeyState, "KeyState"},
	})

void InputSystem::Save()
{
	using namespace nlohmann;

	json j;
	for (int i = 0; i < inputs.size(); i++)
	{
		j[INPUTTEXT].push_back({});
		auto input = j[INPUTTEXT].back();

		input[CODETEXT] = inputs[i]->keyCode;
		input[NAMETEXT] = inputs[i]->name;
		input[TYPETEXT] = inputs[i]->GetType();
	}

	std::ofstream oFile;
	oFile.open(INPUTPATH, std::ios_base::trunc);
	oFile << j;
	oFile.close();
}

InputSystem::InputSysException::InputSysException(int line, const char* file, std::wstring discription) noexcept
	:
	CornDiscriptionException(line, file, discription)
{}

const wchar_t* InputSystem::InputSysException::GetType() const noexcept
{
	return L"Σ(°△°|||) Input system exception";
}
