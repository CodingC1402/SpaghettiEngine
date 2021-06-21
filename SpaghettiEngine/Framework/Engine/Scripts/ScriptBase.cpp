﻿#include "ScriptBase.h"

ScriptTypes* ScriptFactory::map = nullptr;


ScriptTypes* ScriptFactory::GetMap()
{
	if (!map)
		map = new ScriptTypes();
	return map;
}

PScriptBase ScriptFactory::CreateInstance(std::string const& typeName, PScene owner)
{
	const auto iterator = map->find(typeName);
	if (iterator == GetMap()->end())
		return nullptr;
	return static_cast<PScriptBase>(iterator->second(owner));
}

ScriptBase::ScriptException::ScriptException(int line, const char* file, PScriptBase errorScript, const std::wstring& extraDescription)
	: CornException(line, file),
	_errorScript(errorScript),
	_extraDescription(extraDescription)
{}

const wchar_t* ScriptBase::ScriptException::GetType() const noexcept
{
	return L"(╬ Ò﹏Ó) Script format error";
}

const wchar_t* ScriptBase::ScriptException::What() const noexcept
{
	std::wostringstream os;
	os << GetOriginString().c_str() << std::endl;
	os << L"[Script type] " << _errorScript->_name.c_str() << std::endl;
	os << L"[Error] " << L"Wrong format" << std::endl;
	os << L"[Extra description] " << _extraDescription.c_str();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

ScriptBase::ScriptBase(PScene owner, bool isDisabled)
	:
	BaseComponent(owner, isDisabled)
{}

void ScriptBase::AssignOwner(const PGameObj& owner)
{
	if (owner)
		owner->RemoveScript(this);
	owner->AddScript(this);
	_ownerObj = owner;
}

const char* ScriptBase::GetName() const noexcept
{
	return  _name.c_str();
}

Matrix4 ScriptBase::GetWorldMatrix() const noexcept
{
	return _ownerObj->GetWorldMatrix();
}

Vector3 ScriptBase::GetWorldTransform() const noexcept
{
	return _ownerObj->GetWorldTransform();
}

Vector3 ScriptBase::GetWorldRotation() const noexcept
{
	return _ownerObj->GetWorldRotation();
}

Vector3 ScriptBase::GetWorldScale() const noexcept
{
	return _ownerObj->GetWorldScale();
}

WGameObj ScriptBase::GetGameObject() const noexcept
{
	return std::dynamic_pointer_cast<GameObj>(_ownerObj->GetSharedPtr());
}

void ScriptBase::Load(nlohmann::json& input)
{
	if (_ownerObj == nullptr)
		throw CORN_EXCEPT_WITH_DESCRIPTION(L"You can't have a script without an owner");
}

Scene::SBaseComponent ScriptBase::Clone()
{
	auto cloneScript = std::make_shared<ScriptBase>(ScriptFactory::CreateInstance(_name, _owner));
	*cloneScript.get() = *this;
	return cloneScript;
}

void ScriptBase::Destroy()
{
	if (!_isDisabled)
	{
		OnEnd();
		Disable();
	}
	BaseComponent::Destroy();
}
