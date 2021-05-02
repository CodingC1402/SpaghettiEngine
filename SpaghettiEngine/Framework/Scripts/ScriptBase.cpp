#include "ScriptBase.h"

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

PScriptBase ScriptFactory::CopyInstance(CPScriptBase instance)
{
	PScriptBase copyScript = CreateInstance(instance->GetName(), instance->GetOwner());
	copyScript->Copy(instance);
	return copyScript;
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
	os << L"[Script type] " << _errorScript->name.c_str() << std::endl;
	os << L"[Error] " << L"Wrong format" << std::endl;
	os << L"[Extra description] " << _extraDescription.c_str();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

ScriptBase::ScriptBase(PScene owner)
	:
	BaseComponent(owner)
{}

const char* ScriptBase::GetName() const noexcept
{
	return  name.c_str();
}

Matrix ScriptBase::GetWorldMatrix() noexcept
{
	return owner->GetWorldMatrix();
}

Vector3 ScriptBase::GetTransform() const noexcept
{
	return owner->GetWorldTransform();
}

bool ScriptBase::Copy(CPScriptBase script)
{
	return this->name == script->name;
}

void ScriptBase::Unload()
{
	if (!isDisabled)
		OnDisabled();
}
