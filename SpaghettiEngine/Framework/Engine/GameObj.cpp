#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include "GraphicsMath.h"
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

void GameObj::End() const
{
	for (const auto& script : scripts)
		script->End();
}

PGameObj GameObj::GetParent() const
{
	return parent;
}

PGameObj GameObj::GetChild(UINT index) const
{
	if (index >= children.size())
		return nullptr;

	auto iterator = children.begin();
	std::advance(iterator, index);
	return *iterator;
}

PScriptBase GameObj::GetScript(UINT index) const noexcept
{
	if (index >= scripts.size())
		return nullptr;

	auto iterator = scripts.begin();
	std::advance(iterator, index);
	return *iterator;
}

PScriptBase GameObj::GetScript(const std::string* name) const noexcept
{
	for (const auto& script : scripts)
	{
		if (script->GetName() == *name)
			return script;
	}
	return nullptr;
}

std::list<PScriptBase> GameObj::GetAllScripts(const std::string* name) const noexcept
{
	std::list<PScriptBase> rList;
	for (const auto& script : scripts)
	{
		if (script->GetName() == *name)
			rList.push_back(script);
	}
	return rList;
}

Matrix GameObj::GetTransformMatrix() const
{
	if (parent != nullptr)
		return this->_transformMatrix * parent->GetTransformMatrix();
	else
		return this->_transformMatrix;
}

Vector3 GameObj::GetPosition() const
{
	return Vector3(GetTransformMatrix());
}

std::string GameObj::GetTag() const
{
	return tag;
}

std::string GameObj::GetPath() const
{
	return path;
}

bool GameObj::IsDisabled() const
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

void GameObj::Translate(const Vector3& vector)
{
	GraphicsMath::TranslateMatrix(_transformMatrix ,vector);
	for (const auto& child : children)
		child->Translate(vector);
}

void GameObj::Translate(const float& x, const float& y, const float& z)
{
	GraphicsMath::TranslateMatrix(_transformMatrix, x, y, z);
	for (const auto& child : children)
		child->Translate(x, y, z);
}

void GameObj::Move(const Vector3& newPosition)
{
	Vector3 delta = newPosition;
	delta.x -= _transformMatrix._41;
	delta.y -= _transformMatrix._42;
	delta.z -= _transformMatrix._43;
	GraphicsMath::MoveMatrix(_transformMatrix, newPosition);
	for (const auto& child : children)
		child->Translate(delta);
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
	Matrix parentMatrix = gameObj->GetTransformMatrix();
	_transformMatrix /= ;
	parent->AddChild(this);
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

void GameObj::SetTag(const char* newTag)
{
	this->tag = newTag;
}

void GameObj::AddScript(const std::string& scriptName, const std::string& arg)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName);
	newScript->owner = this;
	newScript->Load(&arg);
	scripts.push_back(newScript);
}

void GameObj::AddScript(const PScriptBase& script)
{
	scripts.push_back(script);
}

GameObj::GameObj(const GameObj& obj)
	:
	ownerScene(nullptr),
	parent(obj.parent),
	loaded(obj.loaded),
	path(obj.path),
	tag(obj.tag),
	_transformMatrix(obj._transformMatrix)
{
	for (const auto& child : obj.children)
		children.push_back(new GameObj(*child));

	for (PScriptBase copyScript; const auto& script : obj.scripts)
	{
		copyScript = ScriptFactory::CopyInstance(script);
		copyScript->owner = this;
		scripts.push_back(copyScript);
	}
}

GameObj::GameObj(const std::string& path, const PScene& ownerScene)
	:
	ownerScene(ownerScene),
	parent(nullptr),
	path(path)
{
	GraphicsMath::ZeroMatrix(&_transformMatrix);
}

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

		tag = jsonFile["Tag"].get<std::string>();

		_transformMatrix._41 = jsonFile["Position"][0].get<float>();
		_transformMatrix._42 = jsonFile["Position"][1].get<float>();
		_transformMatrix._43 = jsonFile["Position"][2].get<float>();
		
		for (PGameObj newChild; const auto& child : jsonFile["Children"])
		{
			newChild = new GameObj(child.get<std::string>(), ownerScene);
			this->AddChild(newChild);
			newChild->parent = this;
			newChild->Load();
			newChild->Translate(this->GetPosition());
		}

		for (const auto& script : jsonFile["Scripts"])
		{
			AddScript(script["Name"].get<std::string>(), script["Input"].get<std::string>());
		}

		file.close();
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
	End();
	
	for (const auto& child : children)
		child->Destroy();
	children.clear();

	for (auto& script : scripts)
	{
		script->Unload();
		script->Destroy();
	}
	scripts.clear();
	delete this;
}

void GameObj::AddChild(PGameObj child)
{
	children.push_back(child);
}

void GameObj::CalculateWorldMatrix()
{
	_worldMatrix = _scaleMatrix * _rotationMatrix * _transformMatrix;
}

void GameObj::RemoveChild(PGameObj child)
{
	children.remove(child);
}
