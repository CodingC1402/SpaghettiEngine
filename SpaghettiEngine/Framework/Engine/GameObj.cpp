#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include <fstream>

void GameObj::Start()
{
	if (isDisabled)
		return;

	size_t i = scripts.size();
	auto script = scripts.begin();
	while (i > 0)
	{
		(*script)->Start();
		std::advance(script, 1);
		i--;
	}

	i = children.size();
	auto itChild = children.begin();
	while (i > 0)
	{
		(*itChild)->Start();
		std::advance(itChild, 1);
		i--;
	}
}

void GameObj::Update()
{
	if (isDisabled)
		return;

	size_t i = scripts.size();
	auto script = scripts.begin();
	while (i > 0)
	{
		(*script)->Update();
		std::advance(script, 1);
		i--;
	}

	i = children.size();
	auto itChild = children .begin();
	while (i > 0)
	{
		(*itChild)->Update();
		std::advance(itChild, 1);
		i--;
	}
}

void GameObj::End()
{
	throw EXCEPTION_ACCESS_VIOLATION;
}

const PGameObj GameObj::GetParent()
{
	return parent;
}

const PGameObj GameObj::GetChild(UINT index)
{
	if (index >= children.size())
		return nullptr;

	auto iterator = children.begin();
	std::advance(iterator, index);
	return *iterator;
}

const SScriptBase& GameObj::GetScript(UINT index)
{
	if (index >= scripts.size())
		return nullptr;

	auto iterator = scripts.begin();
	std::advance(iterator, index);
	return *iterator;
}

const Vector3& GameObj::GetPosition()
{
	return position;
}

const char* GameObj::GetTag()
{
	return tag.c_str();
}

const char* GameObj::GetPath()
{
	return path.c_str();
}

bool GameObj::IsDisabled()
{
	return isDisabled;
}

void GameObj::Disable()
{
	isDisabled = true;
	size_t size = scripts.size();
	auto iterator = scripts.begin();
	while (size > 0)
	{
		(*iterator)->OnDisabled();
		std::advance(iterator, 1);
		size--;
	}
}

void GameObj::Enable()
{
	isDisabled = false;
	size_t size = scripts.size();
	auto iterator = scripts.begin();
	while (size > 0)
	{
		(*iterator)->OnEnabled();
		std::advance(iterator, 1);
		size--;
	}
}

void GameObj::Translate(const Vector3& vector)
{
	position += vector;
	size_t size = children.size();
	auto iterator = children.begin();
	while (size > 0)
	{
		(*iterator)->Translate(vector);
		std::advance(iterator, 1);
		size--;
	}
}

void GameObj::Move(const Vector3& newPosition)
{
	Vector3 delta = newPosition - position;
	position = newPosition;
	size_t size = children.size();
	auto iterator = children.begin();
	while (size > 0)
	{
		(*iterator)->Translate(delta);
		std::advance(iterator, 1);
		size--;
	}
}

void GameObj::RemoveParent()
{
	if (ownerScene)
		ownerScene->AddGameObject(this);

	parent->RemoveChild(this);
	parent = nullptr;
}

void GameObj::AddParent(const PGameObj& gameObj)
{
	if (ownerScene)
		ownerScene->RemoveGameObject(this);

	parent = gameObj;
	gameObj->AddChild(this);
}

void GameObj::BecomeCurrentSceneObj()
{
	if (ownerScene)
		ownerScene->RemoveGameObject(this);

	SceneManager::GetCurrentScene()->AddGameObject(this);
}

void GameObj::BecomeConstSceneObj()
{
	if (ownerScene)
		ownerScene->RemoveGameObject(this);

	SceneManager::GetConstScene()->AddGameObject(this);
}

void GameObj::SetTag(const char* tag)
{
	this->tag = tag;
}

GameObj::~GameObj()
{
	int size = children.size();
	auto iterator = children.begin();

}

void GameObj::AddScript(const std::string& scriptName, const std::string* arg, int argSize)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName);
	newScript->owner = this;
	newScript->Load(arg, argSize);
	scripts.push_back(SScriptBase(newScript));
}

void GameObj::AddScript(const PScriptBase script)
{
	scripts.push_back(SScriptBase(script));
}

GameObj::GameObj(const GameObj& obj)
{
	parent = obj.parent;
	path = obj.path;
	tag = obj.tag;
	loaded = obj.loaded;
	position = obj.position;
	size_t size = obj.children.size();
	auto itChild = obj.children.begin();
	while (size > 0)
	{
		children.push_back(new GameObj(*(*itChild)));
		std::advance(itChild, 1);
		size--;
	}

	size = obj.scripts.size();
	auto itScript = obj.scripts.begin();
	PScriptBase copyScript;
	while (size > 0)
	{
		copyScript = ScriptFactory::CopyInstance(itScript->get());
		copyScript->owner = this;
		scripts.push_back(SScriptBase(copyScript));
		std::advance(itScript, 1);
		size--;
	}
}

GameObj::GameObj(const std::string path, const PScene ownerScene)
	:
	parent(nullptr),
	path(path),
	ownerScene(ownerScene)
{}

#define CHILDREN "Children"
#define SIZE "Size"
#define NAME "Name"
void GameObj::Load()
{
	if (loaded)
		return;

	loaded = true;
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

		int childSize = jsonFile[CHILDREN][SIZE].get<int>();
		char childIndex;
		PGameObj child;
		for (int i = 0; i < childSize; i++)
		{
			childIndex = static_cast<char>('0' + i);
			child = new GameObj(jsonFile[CHILDREN][SIZE][childIndex].get<std::string>(), ownerScene);
			this->AddChild(child);
			child->parent = this;
			child->Load();
			child->position += this->position;
		}

		position.x = jsonFile["X"].get<int>();
		position.y = jsonFile["Y"].get<int>();
		position.z = jsonFile["Z"].get<int>();

		int size = jsonFile["Size"].get<int>();
		char index;
		char argIndex;
		std::string* tempArgInput;
		for (int i = 0; i < size; i++)
		{
			index = static_cast<char>('0' + i);

			int inputArgSize = jsonFile[index][SIZE].get<int>();
			tempArgInput = new std::string[inputArgSize];
			for (int arg = 0; arg < inputArgSize; arg++)
			{
				argIndex = static_cast<char>('0' + arg);
				tempArgInput[arg] = jsonFile[index][arg].get<std::string>();
			}
			AddScript(jsonFile[index]["Name"].get<std::string>(), tempArgInput, inputArgSize);
			delete[] tempArgInput;
		}
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" doesn't have the right format" << std::endl;
		os << L"Exception: " << e.what();

		throw CORN_EXCEPT_WITH_DISCRIPTION(os.str());
	}
}

void GameObj::Destroy()
{
	size_t size = children.size();
	auto itChild = children.begin();
	while (size > 0)
	{
		(*itChild)->Destroy();
		std::advance(itChild, 1);
		size--;
	}
	children.clear();
	size = scripts.size();
	auto itScript = scripts.begin();
	while (size > 0)
	{
		(*itScript)->Unload();
		std::advance(itScript, 1);
		size--;
	}
	delete this;
}

void GameObj::AddChild(PGameObj obj)
{
	children.push_back(obj);
}

void GameObj::RemoveChild(PGameObj obj)
{
	size_t size = children.size();
	auto itChild = children.begin();
	while (size > 0)
	{
		if ((*itChild) == obj)
		{
			children.erase(itChild);
			return;
		}
		std::advance(itChild, 1);
		size--;
	}
}

void GameObj::CallDisableEvent()
{
}

void GameObj::CallEnableEvent()
{
}
