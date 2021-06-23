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

PGameObj GameObj::Clone() const
{
	auto cloneObj = GetOwner()->CreateGameObject(false);

	cloneObj->_tag = _tag;
	cloneObj->_name = _name;
	cloneObj->_children = _children;

	auto& scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		cloneObj->GetScriptContainer().AddItem((*it)->Clone());
	}

	auto& childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		cloneObj->GetChildContainer().AddItem((*it)->Clone());
	}

	return cloneObj;
}

#pragma region Set
void GameObj::SetParentDisability(bool value)
{
	_isParentDisabled = value;
}
void GameObj::SetContainerIterator(std::list<PGameObj>::iterator it)
{
	_containerIterator = it;
}
BaseComponent::Type GameObj::GetComponentType() const
{
	return BaseComponent::Type::gameObj;
}
void GameObj::SetParent(PGameObj parent)
{
	if (!parent && !_parent)
		return;

	// if parent is null then that mean that you have to become root, let the parent do that
	// if your current parent is null then and the new parent null then it will return already.
	if (parent)
		parent->GetChildContainer().AddItem(this);
	else
		_parent->GetChildContainer().RemoveItem(this);
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

#pragma region Get
bool GameObj::GetParentDisability()
{
	return _isParentDisabled;
}
PhysicComponent& GameObj::GetPhysicComponent()
{
	return _physic;
}
std::list<PGameObj>::iterator GameObj::GetContainerIterator() const
{
	return _containerIterator;
}
void GameObj::SetIsRoot(bool value)
{
	_isRoot = value;
}
void GameObj::SetParentInternally(PGameObj obj)
{
	bool before = IsDisabled();

	_parent = obj;
	bool after = BaseComponent::IsDisabled() || obj->IsDisabled();

	if (before != after) // only call if there is a different in disability
	{
		if (after)
			OnDisabled();
		else
			OnEnabled();
	}
	_isParentDisabled = obj->IsDisabled();
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
ScriptContainer& GameObj::GetScriptContainer()
{
	return _scripts;
}
PGameObj GameObj::GetParent() const
{
	return _parent;
}
Transform& GameObj::GetTransform()
{
	return _transform;
}
bool GameObj::IsDisabled() const
{
	return BaseComponent::IsDisabled() || (GetParent() && _isParentDisabled);
}
bool GameObj::IsRoot() const
{
	return _isRoot;
}
#pragma endregion

#pragma region Constructor/Destructor

GameObj::GameObj(PScene owner, bool isDisabled)
	:
	_physic(this),
	_children(this),
	_scripts(this),
	BaseComponent(owner, isDisabled)
{}

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
			dynamic_cast<PGameObj>(GetOwner()->GetComponent(child[Field::idField].get<CULL>()))->SetParent(this);

		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::scriptsField;

		for (const auto& script : input[Field::scriptsField])
			dynamic_cast<ScriptBase*>(GetOwner()->GetComponent(script[Field::idField].get<CULL>()))->SetGameObject(this);
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