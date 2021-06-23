#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include "ScriptBase.h"
#include "Scene.h"
#include "LoadingJson.h"
#include "Setting.h"
#include "SMath.h"
#include "ContainerUtil.h"
#include "Physic.h"
#include <fstream>

nlohmann::json GameObj::defaultJson = {
	{LoadingJson::Field::transformField, {0, 0, 0}},
	{LoadingJson::Field::rotationField, {0, 0, 0}},
	{LoadingJson::Field::scaleField, {0, 0, 0}},
	{LoadingJson::Field::gameObjectsField},
	{LoadingJson::Field::scriptsField},
	{LoadingJson::Field::tagField, "new"}
};

SGameObj GameObj::Clone() const
{
	auto cloneObj = _owner->CreateGameObject();

	cloneObj->_tag = _tag;
	cloneObj->_name = _name;
	cloneObj->_children = _children;

	for (const auto& script : _scripts)
		cloneObj->AddScriptClone(script.lock().get());

	return cloneObj;
}

#pragma region Get
PScriptBase GameObj::GetScript(unsigned index) const noexcept
{
	if (index >= _scripts.size())
		return nullptr;

	auto iterator = _scripts.begin();
	std::advance(iterator, index);
	return (*iterator).lock().get();
}
PScriptBase GameObj::GetScript(const std::string& name) const noexcept
{
	for (const auto& script : _scripts)
	{
		if (script.lock()->GetType() == name)
			return script.lock().get();
	}
	return nullptr;
}
std::list<PScriptBase> GameObj::GetAllScripts(const std::string& name) const noexcept
{
	std::list<PScriptBase> rList;
	for (const auto& script : _scripts)
	{
		if (script.lock()->GetType() == name)
			rList.emplace_back(script.lock().get());
	}
	return rList;
}
std::string GameObj::GetTag() const
{
	return _tag;
}
std::string GameObj::GetName() const
{
	return _name;
}
ChildContainer& GameObj::GetChildContainer()
{
	return _children;
}
Transform& GameObj::GetTransform()
{
	return _transform;
}
bool GameObj::IsDestroyed() const
{
	return _isReadyForDelete;
}
void GameObj::SetParent(PGameObj parent)
{
	if (!_parent)
		_parent->GetChildContainer().RemoveItem(this);
	_parent = parent;

	if(_parent)
		_parent->GetChildContainer().AddItem(this);
}
void GameObj::SetName(const std::string& name)
{
	_name = name;
}
void GameObj::SetTag(const std::string& tag)
{
	_tag = tag;
}
#pragma endregion

PhysicComponent& GameObj::GetPhysicComponent()
{
	return _physic;
}

#pragma region Constructor/Destructor

GameObj::GameObj(PScene owner, bool isDisabled)
	:
	_physic(this),
	_children(this),
	BaseComponent(owner, isDisabled)
{}

GameObj::GameObj(GameObj & obj)
	:
	_physic(this),
	_children(this),
	BaseComponent(obj._owner, obj.IsDisabled())
{
	*this = obj;
}

GameObj& GameObj::operator=(GameObj& obj)
{
	if (obj.IsDisabled())
		Disable();
	else
		Enable();

	auto cloneObj = std::dynamic_pointer_cast<GameObj>(obj.Clone());
	auto cloneObjPtr = cloneObj.get();

	_scripts = cloneObjPtr->_scripts;
	_children = cloneObjPtr->_children;

	return *this;
}

#pragma endregion 
void GameObj::Load(nlohmann::json& input)
{
	if (_loaded)
		return;

	_loaded = true;
	using namespace nlohmann;
	using namespace LoadingJson;
	// Use to track which field so it can descibe precisely the error
	std::string fieldTracker = "Start of the file";
	
	try
	{
		_tag = input[Field::tagField].get<std::string>();
		// use to check which field throw error
		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::transformField;
		_transform.SetTransform(input[Field::transformField][0].get<float>(),
								input[Field::transformField][1].get<float>(),
								input[Field::transformField][2].get<float>());
		
		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::rotationField;
		_transform.SetRotation(	input[Field::rotationField][0].get<float>(),
								input[Field::rotationField][1].get<float>(),
								input[Field::rotationField][2].get<float>());
		
		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::scaleField;
		_transform.SetScale(input[Field::scaleField][0].get<float>(),
							input[Field::scaleField][1].get<float>(),
							input[Field::scaleField][2].get<float>());

		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::gameObjectsField;

		for (const auto& child : input[Field::gameObjectsField])
			dynamic_cast<PGameObj>(_owner->GetComponent(child[Field::idField].get<CULL>()).get())->SetParent(this);

		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::scriptsField;

		for (const auto& script : input[Field::scriptsField])
			dynamic_cast<ScriptBase*>(_owner->GetComponent(script[Field::idField].get<CULL>()).get())->AssignOwner(this);
	}
	catch (const CornException&)
	{
		throw;
	}
	catch (const std::exception& e)
	{
		throw GAMEOBJ_FORMAT_EXCEPT(fieldTracker.c_str(), this, e.what());
	}
}