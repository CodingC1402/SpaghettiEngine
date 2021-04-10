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
	auto iterator = map->find(typeName);
	if (iterator == GetMap()->end())
		return nullptr;
	return static_cast<PScriptBase>(iterator->second());
}

PScriptBase ScriptFactory::CopyInstance(const PScriptBase instance)
{
	PScriptBase copyScript = CreateInstance(instance->GetName());
	copyScript->Copy(instance);
	return copyScript;
}

const char* ScriptBase::GetName()
{
	return  name.c_str();
}

bool ScriptBase::Copy(const PScriptBase script)
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
