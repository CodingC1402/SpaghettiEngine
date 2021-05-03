#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include "GraphicsMath.h"
#include "ScriptBase.h"
#include "Scene.h"
#include "Path.h"
#include "Prefabs.h"
#include "Setting.h"

#include <fstream>

constexpr auto tagField			= "Tag";
constexpr auto transformField	= "Transform";
constexpr auto rotationField	= "Rotation";
constexpr auto scaleField		= "Scale";
constexpr auto scriptsField	= "Scripts";
constexpr auto childrenField	= "Children";

nlohmann::json GameObj::defaultJson = {
	{transformField, {0, 0, 0}},
	{rotationField, {0, 0, 0}},
	{scaleField, {0, 0, 0}},
	{childrenField},
	{scriptsField},
	{tagField, "new"}
};

#pragma region Get
Matrix GameObj::GetWorldMatrix()
{
	CalculateWorldMatrix();
	return _worldMatrix;
}
Vector3 GameObj::GetWorldTransform() const
{
	if (parent != nullptr)
		return _transform + parent->GetWorldTransform();
	else
		return _transform;
}
Vector3 GameObj::GetWorldRotation() const
{
	if (parent != nullptr)
		return _rotation + parent->GetWorldRotation();
	else
		return _rotation;
}
Vector3 GameObj::GetWorldScale() const
{
	Vector3 rValue = _scale;
	if (parent != nullptr)
	{
		const Vector3 parentValue = parent->GetWorldScale();
		rValue = Vector3(rValue.x * parentValue.x, rValue.y * parentValue.y, rValue.y * parentValue.y);
	}
	return rValue;
}
Vector3 GameObj::GetTransform() const
{
	return _transform;
}
Vector3 GameObj::GetRotation() const
{
	return _rotation;
}
Vector3 GameObj::GetScale() const
{
	return _scale;
}
PGameObj GameObj::GetParent() const
{
	return parent;
}
PGameObj GameObj::GetChild(UINT index) const
{
	if (index >= _children.size())
		return nullptr;

	auto iterator = _children.begin();
	std::advance(iterator, index);
	return *iterator;
}
PScriptBase GameObj::GetScript(UINT index) const noexcept
{
	if (index >= _scripts.size())
		return nullptr;

	auto iterator = _scripts.begin();
	std::advance(iterator, index);
	return *iterator;
}
PScriptBase GameObj::GetScript(const std::string& name) const noexcept
{
	for (const auto& script : _scripts)
	{
		if (script->GetName() == name)
			return script;
	}
	return nullptr;
}
std::list<PScriptBase> GameObj::GetAllScripts(const std::string& name) const noexcept
{
	std::list<PScriptBase> rList;
	for (const auto& script : _scripts)
	{
		if (script->GetName() == name)
			rList.emplace_back(script);
	}
	return rList;
}
std::string GameObj::GetTag() const
{
	return tag;
}
std::string GameObj::GetPath() const
{
	return path;
}
#pragma endregion
#pragma region Set
void GameObj::SetTag(const char* newTag)
{
	this->tag = newTag;
}
void GameObj::SetRotation(const float& x, const float& y, const float& z)
{
	SetRotation(Vector3(x, y, z));
}
void GameObj::SetRotation(const Vector3& vec3)
{
	if (_rotation == vec3)
	{
		_rotation = vec3;
		GraphicsMath::Modulo(_rotation, 360.0f);
		_isRotationChanged = true;
		ForceRecalculateMatrix();
	}
}
void GameObj::SetScale(const float& x, const float& y, const float& z)
{
	SetScale(Vector3(x, y, z));
}
void GameObj::SetScale(const Vector3& vec3)
{
	if (_scale != vec3)
	{
		_scale.x = vec3.x;
		_scale.y = vec3.y;
		_scale.z = vec3.z;
		_isScaleChanged = true;
		ForceRecalculateMatrix();
	}
}
void GameObj::SetTransform(const float& x, const float& y, const float& z)
{
	SetTransform(Vector3(x, y, z));
}
void GameObj::SetTransform(const Vector3& vec3)
{
	if (_transform != vec3)
	{
		_transform = vec3;
		_isTransformChanged = true;
		ForceRecalculateMatrix();
	}
}
void GameObj::Rotate(const float& x, const float& y, const float& z)
{
	Rotate(Vector3(x, y, z));
}
void GameObj::Rotate(const Vector3& rotation)
{
	if (rotation == Vector3(0, 0, 0))
		return;
	
	_rotation += rotation;
	GraphicsMath::Modulo(_rotation, 360.0f);
	_isRotationChanged = true;
	ForceRecalculateMatrix();
}
void GameObj::Translate(const float& x, const float& y, const float& z)
{
	Translate(Vector3(x, y, z));
}
void GameObj::Translate(const Vector3& vector)
{
	if (vector == Vector3(0, 0, 0))
		return;
	
	_transform += vector;
	_isTransformChanged = true;
	ForceRecalculateMatrix();
}
void GameObj::ForceRecalculateMatrix()
{
	this->_isChanged = true;
	for (const auto& child : _children)
		child->_isChanged = true;
}
#pragma endregion
#pragma region Parent methods
void GameObj::RemoveParent()
{
	if (!parent)
		return;
	
	_owner->PromoteGameObjToRoot(this);
	_transform += parent->GetWorldTransform();
	_rotation  += parent->GetWorldRotation();
	const Vector3 parentScale = parent->GetWorldScale();
	_scale.x *= parentScale.x;
	_scale.y *= parentScale.y;
	_scale.z *= parentScale.z;

	_isTransformChanged = true;
	_isRotationChanged = true;
	_isScaleChanged = true;
	ForceRecalculateMatrix();
	
	parent->RemoveChild(this);
	parent = nullptr;
}
void GameObj::AddParent(const PGameObj& gameObj)
{
	if (parent)
		RemoveParent();

	parent = gameObj;
	_owner->DemoteGameObjFromRoot(this);
	_transform -= parent->GetWorldTransform();
	_rotation  -= parent->GetWorldRotation();
	const Vector3 parentScale = parent->GetWorldScale();
	_scale.x /= parentScale.x;
	_scale.y /= parentScale.y;
	_scale.z /= parentScale.z;

	_isTransformChanged = true;
	_isRotationChanged = true;
	_isScaleChanged = true;
	ForceRecalculateMatrix();
	
	parent->AddChild(this);
}
#pragma endregion

GameObj::GameObj(PScene owner, bool isDisabled)
	:
	BaseComponent(owner, isDisabled)
{}

GameObj::~GameObj()
{
	for (const auto& child : _children)
		child->OnEnd();
	for (const auto& child : _children)
		child->Destroy();
	_children.clear();

	for (auto& script : _scripts)
	{
		script->Unload();
		script->Destroy();
	}
	_scripts.clear();
}

#pragma region Scripts
PScriptBase GameObj::AddScript(const std::string& scriptName, nlohmann::json& inputObject)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName, _owner);
	newScript->Load(inputObject);
	_scripts.push_back(newScript);
	return _scripts.back();
}
PScriptBase GameObj::AddScript(const PScriptBase& script)
{
	_scripts.push_back(script);
	return _scripts.back();
}
#pragma endregion 
#pragma region Constructors

#pragma endregion
void GameObj::Load(nlohmann::json& input)
{
	if (loaded)
		return;

	loaded = true;
	using namespace nlohmann;
	// Use to track which field so it can descibe precisely the error
	std::string fieldTracker = "Start of the file";
	
	try
	{
		constexpr auto idField			= "ID";
		constexpr auto gameObjsField	= "GameObjects";
		constexpr auto isRootField		= "IsRoot";
		
		tag = input[tagField].get<std::string>();
		// use to check which field throw error
		if constexpr (Setting::IsDebugMode())
			fieldTracker = transformField;
		_transform.x = input[transformField][0].get<float>();
		_transform.y = input[transformField][1].get<float>();
		_transform.z = input[transformField][2].get<float>();
		
		if constexpr (Setting::IsDebugMode())
			fieldTracker = rotationField;
		_rotation.x = input[rotationField][0].get<float>();
		_rotation.y = input[rotationField][1].get<float>();
		_rotation.z = input[rotationField][2].get<float>();
		
		if constexpr (Setting::IsDebugMode())
			fieldTracker = scaleField;
		_scale.x = input[scaleField][0].get<float>();
		_scale.y = input[scaleField][1].get<float>();
		_scale.z = input[scaleField][2].get<float>();

		_isTransformChanged = true;
		_isRotationChanged = true;
		_isScaleChanged = true;
		_isChanged = true;

		if constexpr (Setting::IsDebugMode())
			fieldTracker = childrenField;
		for (const auto & child : input[childrenField])
		{
			_children.push_back(dynamic_cast<PGameObj>(_owner->GetComponent(child.get<CULL>()).get()));
			_children.back()->parent = this;
		}

		if constexpr (Setting::IsDebugMode())
			fieldTracker = scriptsField;
		for (const auto& script : input[scriptsField])
		{
			_scripts.push_back(dynamic_cast<PScriptBase>(_owner->GetComponent(script.get<CULL>()).get()));
			_scripts.back()->_ownerObj = this;
		}
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
	OnEnd();

	if (parent)
		parent->RemoveChild(this);
	
	BaseComponent::Destroy();
}
#pragma region Obj Event
void GameObj::OnStart()
{
	if (isDisabled)
		return;

	OnEnabled();
	for (const auto& script : _scripts)
		script->OnStart();

	for (const auto& child : _children)
		child->OnStart();
}
void GameObj::OnUpdate()
{
	if (isDisabled)
		return;

	for (const auto& script : _scripts)
		script->OnUpdate();

	for (const auto& child : _children)
		child->OnUpdate();
}
void GameObj::OnEnd()
{
	for (const auto& script : _scripts)
		script->OnEnd();
}
void GameObj::OnEnabled()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script->OnEnabled();

	for (const auto& child : _children)
		child->OnEnabled();
}

void GameObj::OnDisabled()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script->OnDisabled();

	for (const auto& child : _children)
		child->OnDisabled();
}
#pragma endregion
#pragma region  Child
PGameObj GameObj::AddChild(PGameObj child)
{
	_children.push_back(child);
	return _children.back();
}

PGameObj GameObj::AddChild()
{
	_children.push_back(new GameObj(_owner));
	_children.back()->Load(defaultJson);
	return _children.back();
}

Scene::BaseComponent* GameObj::Clone()
{
	throw CORN_EXCEPT_WITH_DESCRIPTION(L"Unimplemented function");
	return nullptr;
}

void GameObj::RemoveChild(PGameObj child)
{
	_children.remove(child);
}
#pragma endregion 
#pragma region Matrix Calculation
void GameObj::CalculateRotationMatrix()
{
	if (!_isRotationChanged)
		return;
	
	const Matrix XAxis = GraphicsMath::GetXAxisRotateMatrix(_rotation.x);
	const Matrix YAxis = GraphicsMath::GetYAxisRotateMatrix(_rotation.y);
	const Matrix ZAxis = GraphicsMath::GetZAxisRotateMatrix(_rotation.z);
	
	_rotationMatrix = XAxis * ZAxis * YAxis;
	_isRotationChanged = false;
}
void GameObj::CalculateTransformMatrix()
{
	if (!_isTransformChanged)
		return;

	_transformMatrix._41 = _transform.x;
	_transformMatrix._42 = _transform.y;
	_transformMatrix._43 = _transform.z;
	_transformMatrix._11 = 1;
	_transformMatrix._22 = 1;
	_transformMatrix._33 = 1;
	_transformMatrix._44 = 1;
	_isTransformChanged = false;
}
void GameObj::CalculateScaleMatrix()
{
	if (!_isScaleChanged)
		return;

	_scaleMatrix._11 = _scale.x;
	_scaleMatrix._22 = _scale.y;
	_scaleMatrix._33 = _scale.z;
	_scaleMatrix._44 = 1;
	_isScaleChanged = false;
}
void GameObj::CalculateWorldMatrix()
{
	if (!_isChanged)
		return;
	
	CalculateRotationMatrix();
	CalculateTransformMatrix();
	CalculateScaleMatrix();
	
	if (parent != nullptr)
		_worldMatrix = _scaleMatrix * _rotationMatrix * _transformMatrix * parent->GetWorldMatrix();
	else
		_worldMatrix = _scaleMatrix * _rotationMatrix * _transformMatrix;

	_isChanged = false;
}
#pragma endregion