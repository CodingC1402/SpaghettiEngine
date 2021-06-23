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

	for (const auto& script : _scriptsPtr)
		cloneObj->AddScriptClone(script.get());

	return cloneObj;
}

#pragma region Get
PScriptBase GameObj::GetScript(unsigned index) const noexcept
{
	if (index >= _scriptsPtr.size())
		return nullptr;

	auto iterator = _scriptsPtr.begin();
	std::advance(iterator, index);
	return (*iterator).get();
}
PScriptBase GameObj::GetScript(const std::string& name) const noexcept
{
	for (const auto& script : _scriptsPtr)
	{
		if (script->GetType() == name)
			return script.get();
	}
	return nullptr;
}
std::list<PScriptBase> GameObj::GetAllScripts(const std::string& name) const noexcept
{
	std::list<PScriptBase> rList;
	for (const auto& script : _scriptsPtr)
	{
		if (script->GetType() == name)
			rList.emplace_back(script.get());
	}
	return rList;
}
std::string GameObj::GetTag() const
{
	return _tag;
}
bool GameObj::IsDestroyed() const
{
	return _isReadyForDelete;
}
void GameObj::SetTag(const std::string& tag)
{
	_tag = tag;
}
void GameObj::AddPhysicComponent(PhysicScriptBase* script)
{
	ContainerUtil::EmplaceBackUnique(_physicComponents, script);
	if (_physicComponents.size() == 1)
		Physic::AddGameObj(this);
}
void GameObj::RemovePhysicComponent(PhysicScriptBase* script)
{
	ContainerUtil::Erase(_physicComponents, script);
	if (_physicComponents.size() == 0)
		Physic::RemoveGameObj(this);
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
	BaseComponent(owner, isDisabled)
{}

GameObj::GameObj(GameObj & obj)
	:
	_physic(this),
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

	_scriptsPtr = cloneObjPtr->_scriptsPtr;

	return *this;
}

#pragma endregion 
void GameObj::Load(nlohmann::json& input)
{
	if (loaded)
		return;

	loaded = true;
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
		_transform.x = input[Field::transformField][0].get<float>();
		_transform.y = input[Field::transformField][1].get<float>();
		_transform.z = input[Field::transformField][2].get<float>();
		
		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::rotationField;
		_rotation.x = input[Field::rotationField][0].get<float>();
		_rotation.y = input[Field::rotationField][1].get<float>();
		_rotation.z = input[Field::rotationField][2].get<float>();
		
		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::scaleField;
		_scale.x = input[Field::scaleField][0].get<float>();
		_scale.y = input[Field::scaleField][1].get<float>();
		_scale.z = input[Field::scaleField][2].get<float>();

		_isTransformChanged = true;
		_isRotationChanged = true;
		_isScaleChanged = true;
		_isChanged = true;

		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::gameObjectsField;
		for (const auto & child : input[Field::gameObjectsField])
			dynamic_cast<PGameObj>(_owner->GetComponent(child[Field::idField].get<CULL>()).get())->AddParentWithoutCalculateLocal(this);

		if constexpr (Setting::IsDebugMode())
			fieldTracker = Field::scriptsField;
		for (const auto& script : input[Field::scriptsField])
			dynamic_cast<ScriptBase*>(_owner->GetComponent(script[Field::idField].get<CULL>()).get())->AssignOwner(this);

		if (!(input[Field::isRootField] == nullptr || !input[Field::isRootField].get<bool>() || parent != nullptr))
			_owner->PromoteGameObjToRoot(this);
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
void GameObj::Destroy()
{

	
	BaseComponent::Destroy();
}