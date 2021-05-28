#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include "ScriptBase.h"
#include "Scene.h"
#include "LoadingJson.h"
#include "Setting.h"
#include "SMath.h"
#include <fstream>

nlohmann::json GameObj::defaultJson = {
	{LoadingJson::Field::transformField, {0, 0, 0}},
	{LoadingJson::Field::rotationField, {0, 0, 0}},
	{LoadingJson::Field::scaleField, {0, 0, 0}},
	{LoadingJson::Field::gameObjectsField},
	{LoadingJson::Field::scriptsField},
	{LoadingJson::Field::tagField, "new"}
};

#pragma region Get
Matrix4 GameObj::GetWorldMatrix()
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
	return  *iterator;
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
		SMath::Modulo(_rotation, 360.0f);
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
	SMath::Modulo(_rotation, 360.0f);
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
	parent->RemoveChild(this);
	
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

	parent = nullptr;
}
void GameObj::AddParent(const PGameObj& gameObj)
{
	if (parent)
		RemoveParent();

	parent = gameObj;
	
	parent->AddChild(this);
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
}

void GameObj::AddParentWithoutCalculateLocal(const PGameObj& gameObj)
{
	if (parent)
		RemoveParentWithoutCalculateLocal();
	
	parent = gameObj;
	parent->AddChild(this);
	_owner->DemoteGameObjFromRoot(this);
	ForceRecalculateMatrix();
}

void GameObj::RemoveParentWithoutCalculateLocal()
{
	if (!parent)
		return;
	
	_owner->PromoteGameObjToRoot(this);
	parent->RemoveChild(this);
	parent = nullptr;
	ForceRecalculateMatrix();
}
#pragma endregion
#pragma region Constructor/Destructor
GameObj::GameObj(PScene owner, bool isDisabled)
	:
	BaseComponent(owner, isDisabled)
{}

void GameObj::RecursiveMarkForDelete()
{
	for(const auto& child : _children)
		child->RecursiveMarkForDelete();
	_isReadyForDelete = true;
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
		tag = input[Field::tagField].get<std::string>();
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
			dynamic_cast<PScriptBase>(_owner->GetComponent(script[Field::idField].get<CULL>()).get())->AssignOwner(this);

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
	if (!_isReadyForDelete)
	{
		OnEnd();
		
		if (parent)
			parent->RemoveChild(this); // This cause invalid access when clear cause Destroy is called each time a obj is destroy;
		else
			_owner->DemoteGameObjFromRoot(this);

		RecursiveClearScriptsWithoutCallEnd();
		RecursiveMarkForDelete();
	}

	_children.clear();
	_childrenPtr.clear();
	
	BaseComponent::Destroy();
}
#pragma region Obj Event
void GameObj::OnStart()
{
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
	for (const auto& child : _children)
		child->OnEnd();
	
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
std::shared_ptr<Scene::BaseComponent> GameObj::Clone()
{
	return nullptr;
}
PGameObj GameObj::AddChild(const PGameObj& child)
{
	_childrenPtr.push_back(child->GetSharedPtr());
	_children.push_back(child);
	return _children.back();
}
PGameObj GameObj::AddChild()
{
	const Scene::SBaseComponent newObj(new GameObj(_owner), Scene::DestroyComponent);
	_childrenPtr.push_back(newObj);
	_children.push_back(dynamic_cast<PGameObj>(_childrenPtr.back().get()));
	_childrenPtr.back()->AssignSharedPtr(_childrenPtr.back());
	_children.back()->Load(defaultJson);
	return _children.back();
}
PScriptBase GameObj::AddScript(const std::string& scriptName, nlohmann::json& inputObject)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName, _owner);
	const Scene::SBaseComponent sharedPtr(newScript, Scene::DestroyComponent);
	newScript->AssignSharedPtr(sharedPtr);
	newScript->Load(inputObject);
	_scriptsPtr.push_back(sharedPtr);
	_scripts.push_back(newScript);
	return _scripts.back();
}
PScriptBase GameObj::AddScript(const PScriptBase& script)
{
	_scriptsPtr.push_back(script->GetSharedPtr());
	_scripts.push_back(script);
	return _scripts.back();
}
void GameObj::RemoveChild(const PGameObj& child)
{
	_childrenPtr.remove(child->GetSharedPtr());
	_children.remove(child);
}

void GameObj::RemoveScript(const PScriptBase& script)
{
	_scriptsPtr.remove(script->GetSharedPtr());
	_scripts.remove(script);
}

void GameObj::RecursiveClearScriptsWithoutCallEnd()
{
	for (const auto& child : _children)
		child->RecursiveClearScriptsWithoutCallEnd();
	ClearScriptsWithoutCallEnd();
}

void GameObj::ClearScriptsWithoutCallEnd()
{
	for (auto& script : _scripts)
		script->Disable();
	_scriptsPtr.clear();
	_scripts.clear();
}

void GameObj::ClearScripts()
{
	for (auto& script : _scripts)
		script->OnEnd();
	ClearScriptsWithoutCallEnd();
}

void GameObj::RecursiveClearScripts()
{
	OnEnd();
	RecursiveClearScriptsWithoutCallEnd();
}

#pragma endregion 
#pragma region Matrix4 Calculation
void GameObj::CalculateRotationMatrix()
{
	if (!_isRotationChanged)
		return;
	
	const Matrix4 XAxis = SMath::GetXAxisRotateMatrix(_rotation.x);
	const Matrix4 YAxis = SMath::GetYAxisRotateMatrix(_rotation.y);
	const Matrix4 ZAxis = SMath::GetZAxisRotateMatrix(_rotation.z);
	
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