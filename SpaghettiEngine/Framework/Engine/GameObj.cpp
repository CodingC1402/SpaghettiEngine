#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include <fstream>

void GameObj::Start()
{

}

void GameObj::Update()
{
	int i = scripts.size();
	auto script = scripts.begin();
	while (i > 0)
	{
		(*script)->Update();
		std::advance(script, 1);
		i--;
	}
}

void GameObj::End()
{
	throw EXCEPTION_ACCESS_VIOLATION;
}

const char* GameObj::GetTag()
{
	return tag.c_str();
}

void GameObj::SetTag(const char* tag)
{
	this->tag = tag;
}

const char* GameObj::GetName()
{
	return name.c_str();
}

void GameObj::SetName(const char* name)
{
	this->name = name;
}

GameObj::~GameObj()
{
	int size = children.size();
	auto iterator = children.begin();

}

void GameObj::AddScript(const std::string& scriptName, const std::string* arg)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName);
	newScript->Load();
	scripts.push_back();
}

void GameObj::Load(const std::string& path)
{
	using namespace nlohmann;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" Doesn't exist";
		throw CORN_EXCEPT_WITH_DISCRIPTION (os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		int size = jsonFile["Size"].get<int>();
		char index;
		char argIndex;
		std::string* tempArgInput;
		for (int i = 0; i < size; i++)
		{
			index = static_cast<char>('0' + i);

			int inputArgSize = jsonFile[index]["Size"].get<int>();
			tempArgInput = new std::string[inputArgSize];
			for (int arg = 0; arg < inputArgSize; arg++)
			{
				argIndex = static_cast<char>('0' + arg);
				tempArgInput[arg] = jsonFile[index][arg].get<std::string>();
			}
			AddScript(jsonFile[index]["Name"].get<std::string>(), tempArgInput);
			delete[] tempArgInput;
		}
	}
	catch (...)
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" doesn't have the right format";
		throw CORN_EXCEPT_WITH_DISCRIPTION(os.str());
	}
}
