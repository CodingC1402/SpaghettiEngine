#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include <fstream>

void GameObj::Start()
{
	if (isDisabled)
		return;

	for (const auto& script : scripts)
		script->Start();

	for (const auto& child : children)
		child->Start();
}

void GameObj::Update()
{
	if (isDisabled)
		return;


	for (const auto& script : scripts)
		script->Update();

	for (const auto& child : children)
		child->Update();
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

const SScriptBase GameObj::GetScript(UINT index)
{
	if (index >= scripts.size())
		return nullptr;

	auto iterator = scripts.begin();
	std::advance(iterator, index);
	return *iterator;
}

const Vector3* GameObj::GetPosition()
{
	return &position;
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
	if (isDisabled)
		return;

	isDisabled = true;
	for (const auto& script : scripts)
		script->OnDisabled();

	for (const auto& child : children)
		child->Disable();
}

void GameObj::Enable()
{
	if (!isDisabled)
		return;

	isDisabled = false;
	for (const auto& script : scripts)
		script->OnEnabled();

	for (const auto& child : children)
		child->Enable();
}

void GameObj::Translate(const Vector3* vector)
{
	position += *vector;
	for (const auto& child : children)
		child->Translate(vector);
}

void GameObj::Move(const Vector3* newPosition)
{
	Vector3 delta = *newPosition - position;
	position = *newPosition;
	for (const auto& child : children)
		child->Translate(&delta);
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

void GameObj::AddScript(const std::string& scriptName, const std::string& arg)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName);
	newScript->owner = this;
	newScript->Load(&arg);
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
	for (const auto& child : obj.children)
		children.push_back(new GameObj(*child));

	PScriptBase copyScript;
	for (const auto& script : obj.scripts)
	{
		copyScript = ScriptFactory::CopyInstance(script.get());
		copyScript->owner = this;
		scripts.push_back(SScriptBase(copyScript));
	}
}

GameObj::GameObj(const std::string path, const PScene ownerScene)
	:
	parent(nullptr),
	path(path),
	ownerScene(ownerScene)
{}

#define NAME "Name"
#define TAG "Tag"
#define POSITION "Position"
#define CHILDREN "Children"
#define SCRIPTS "Scripts"
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

		tag = jsonFile[TAG].get<std::string>();

		position.x = jsonFile[POSITION][0].get<int>();
		position.y = jsonFile[POSITION][1].get<int>();
		position.z = jsonFile[POSITION][2].get<int>();

		PGameObj newChild;
		for (const auto& child : jsonFile[CHILDREN])
		{
			newChild = new GameObj(child.get<std::string>(), ownerScene);
			this->AddChild(newChild);
			newChild->parent = this;
			newChild->Load();
			newChild->Translate(this->GetPosition());
		}

		for (const auto& script : jsonFile[SCRIPTS])
		{
			AddScript(script["Name"].get<std::string>(), script["Input"].get<std::string>());
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
	for (const auto& child : children)
		child->Destroy();
	children.clear();

	for (const auto& script : scripts)
		script->Unload();
	scripts.clear();
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
