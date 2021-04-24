#include "ScriptBase.h"

ScriptTypes* ScriptFactory::map = nullptr;


ScriptTypes* ScriptFactory::GetMap()
{
	if (!map)
		map = new ScriptTypes();
	return map;
}

ScriptBase* ScriptFactory::CreateInstance(std::string const& typeName)
{
	const auto iterator = map->find(typeName);
	if (iterator == GetMap()->end())
		return nullptr;
	return static_cast<PScriptBase>(iterator->second());
}

PScriptBase ScriptFactory::CopyInstance(CPScriptBase instance)
{
	PScriptBase copyScript = CreateInstance(instance->GetName());
	copyScript->Copy(instance);
	return copyScript;
}

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

void ScriptBase::Disable()
{
	if (isDisabled)
		return;
	isDisabled = true;
	OnDisabled();
}

void ScriptBase::Enable()
{
	if (!isDisabled)
		return;
	isDisabled = false;
	OnEnabled();
}

void ScriptBase::Destroy() const
{
	delete this;
}
