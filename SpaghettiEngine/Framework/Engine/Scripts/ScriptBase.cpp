#include "ScriptBase.h"

ScriptTypes* ScriptFactory::map = nullptr;


ScriptTypes* ScriptFactory::GetMap()
{
	if (!map)
		map = new ScriptTypes();
	return map;
}

PScriptBase ScriptFactory::CreateInstance(std::string const& typeName, PScene owner, bool isDisabled)
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
	os << L"[Script type] " << _errorScript->GetType().c_str() << std::endl;
	os << L"[Error] " << L"Wrong format" << std::endl;
	os << L"[Extra description] " << _extraDescription.c_str();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

ScriptBase::ScriptBase(PScene owner, bool isDisabled)
	:
	BaseComponent(owner, isDisabled)
{}

void ScriptBase::SetGameObject(const PGameObj& owner)
{
	if (!owner && !_ownerObj)
		return;

	if (owner)
		owner->GetScriptContainer().AddItem(this);
	else
		_ownerObj->GetScriptContainer().RemoveItem(this);

	if (!(_ownerObj || this->IsDisabled()))
		this->Disable();
}

Matrix4 ScriptBase::GetWorldMatrix() const noexcept
{
	return _ownerObj->GetTransform().GetWorldMatrix();
}

Vector3 ScriptBase::GetWorldTransform() const noexcept
{
	return _ownerObj->GetTransform().GetWorldTransform();
}

Vector3 ScriptBase::GetWorldRotation() const noexcept
{
	return _ownerObj->GetTransform().GetWorldRotation();
}
Vector3 ScriptBase::GetWorldScale() const noexcept
{
	return _ownerObj->GetTransform().GetWorldScale();
}

PGameObj ScriptBase::GetGameObject() const noexcept
{
	return _ownerObj;
}

bool ScriptBase::IsDisabled() const noexcept
{
	return BaseComponent::IsDisabled() || (GetGameObject() && GetGameObject()->IsDisabled());
}

PScriptBase ScriptBase::Clone() const
{
	auto cloneScript = GetOwner()->CreateScriptBase(GetType(), false);
	return cloneScript;
}

void ScriptBase::Load(nlohmann::json& input)
{
	if (_ownerObj == nullptr)
		throw CORN_EXCEPT_WITH_DESCRIPTION(L"You can't have a script without an owner");
}

BaseComponent::Type ScriptBase::GetComponentType() const
{
	return BaseComponent::Type::script;
}

void ScriptBase::Destroy()
{
	if (_ownerObj)
		_ownerObj->GetScriptContainer().RemoveItem(this);

	BaseComponent::Destroy();
}

void ScriptBase::SetContainerIterator(std::list<PScriptBase>::iterator it)
{
	_containerIterator = it;
}

std::list<PScriptBase>::iterator ScriptBase::GetContainerIterator() const
{
	return _containerIterator;
}
