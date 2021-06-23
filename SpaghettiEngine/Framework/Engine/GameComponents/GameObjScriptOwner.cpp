#include "GameObj.h"
#include "ScriptBase.h"

PScriptBase GameObj::AddScript(const std::string& scriptName, nlohmann::json& inputObject)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName, _owner);
	const SBaseComponent sharedPtr(newScript, Scene::DestroyComponent);
	newScript->AssignSharedPtr(sharedPtr);
	newScript->AssignOwner(this);
	newScript->Load(inputObject);
	newScript->OnStart();
	if (!newScript->IsDisabled())
		newScript->OnEnabled();
	_scriptsPtr.push_back(sharedPtr);
	_scripts.push_back(newScript);
	return _scripts.back();
}
PScriptBase GameObj::AddScript(const PScriptBase& script)
{
	_scriptsPtr.push_back(script->GetSharedPtr());
	_scripts.push_back(script);
	script->AssignOwner(this);
	return _scripts.back();
}
PScriptBase GameObj::AddScript(const SScriptBase& script)
{
	return AddScript(script.get());
}
PScriptBase GameObj::AddScriptClone(const PScriptBase& script)
{
	SScriptBase cloneScript = script->Clone();
	return AddScript(cloneScript.get());
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