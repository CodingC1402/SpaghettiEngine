#include "InputSystem.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include <sstream>
#include <fstream>

constexpr const char* InputText = "Inputs";
constexpr const char* TypeText = "Type";
constexpr const char* NameText = "Name";
constexpr const char* CodeText = "KeyCode";

PInputSystem InputSystem::__instance = nullptr;

KeyCode InputSystem::GetFirstKeyPressCode()
{
	return KeyCode();
}

SInput InputSystem::GetInput(const string& name) noexcept
{
	for (const auto& input : __instance->inputs)
		if (input->CheckName(name))
			return input;

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
	for (const auto& input : inputs)
		input->Reset();

	KeyBoard::Event e;
	while (!kb->IsKeyEmpty())
	{
		e = kb->ReadKey();
		if (!e.IsValid())
		{
			for (const auto& input : inputs)
				input->LostFocus();
		}
		else
		{
			for (const auto& input : inputs)
				input->Update(e);
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
		throw INPUTSYS_EXCEPT(oss.str());
	}
	try
	{
		json file;
		jsonStream >> file;
		
		for (const auto& input : file[InputText])
		{
			auto type = input[TypeText].get<Input::Type>();
			auto name = input[NameText].get<std::string>();
			auto code = input[CodeText].get<KeyCode>();
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
	for (const auto& loadedInput : inputs)
	{
		j[InputText].push_back({});
		auto input = j[InputText].back();

		input[CodeText] = loadedInput->keyCode;
		input[NameText] = loadedInput->name;
		input[TypeText] = loadedInput->GetType();
	}

	std::ofstream oFile;
	oFile.open(INPUTPATH, std::ios_base::trunc);
	oFile << j;
	oFile.close();
}

InputSystem::InputSysException::InputSysException(int line, const char* file, std::wstring description) noexcept
	:
	CornDescriptionException(line, file, std::move(description))
{}

const wchar_t* InputSystem::InputSysException::GetType() const noexcept
{
	return L"Σ(°△°|||) Input system exception";
}
