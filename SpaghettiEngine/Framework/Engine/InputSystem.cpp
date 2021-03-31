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
	json file;
	jsonStream >> file;
	int size = file[SIZETEXT].get<int>();

	std::string inputIndex;
	for (int i = 0; i < size; i++)
	{
		inputIndex = INPUTTEXT + std::to_string(i);
		Input::Type type = file[inputIndex.c_str()][TYPETEXT].get<Input::Type>();
		std::string name = file[inputIndex.c_str()][NAMETEXT].get<std::string>();
		KeyCode code = file[inputIndex.c_str()][CODETEXT].get<KeyCode>();
		inputs.push_back(SInput(Input::Create(code, name, type)));
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
	j[SIZETEXT] = inputs.size();

	std::string obj;
	for (int i = 0; i < inputs.size(); i++)
	{
		obj = INPUTTEXT + std::to_string(i);

		j[obj.c_str()][CODETEXT] = inputs[i]->keyCode;
		j[obj.c_str()][NAMETEXT] = inputs[i]->name;
		j[obj.c_str()][TYPETEXT] = inputs[i]->GetType();
	}

	std::ofstream oFile;
	oFile.open(INPUTPATH, std::ios_base::trunc);
	oFile << j;
	oFile.close();
}
