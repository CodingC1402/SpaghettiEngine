#include "ScriptContainer.h"
#include "ScriptBase.h"

ScriptContainer::ScriptContainer(PGameObj owner) : _owner(owner)
{}

PScriptBase ScriptContainer::GetItem(const std::string& name)
{
	for (auto it = _container.begin(); it != _container.end(); ++it)
	{
		if ((*it)->GetName() == name)
		{
			return (*it);
		}
	}
}

PScriptBase ScriptContainer::GetItemType(const std::string& type)
{
	for (auto it = _container.begin(); it != _container.end(); ++it)
	{
		if ((*it)->GetType() == type)
		{
			return (*it);
		}
	}
}

std::list<PScriptBase> ScriptContainer::GetAllItemsType(const std::string& type)
{
	std::list<PScriptBase> rValues;
	for (auto it = _container.begin(); it != _container.end(); ++it)
	{
		if ((*it)->GetType() == type)
			rValues.push_back(*it);
	}
	return rValues;
}

PScriptBase ScriptContainer::AddItemClone(PScriptBase script)
{
	auto clone = script->Clone();
	AddItem(clone);
	return clone;
}

void ScriptContainer::AddItem(PScriptBase script)
{
	if (!script)
		return;

	if (script->GetGameObject())
		script->GetGameObject()->GetScriptContainer().RemoveItem(script);

	script->_ownerObj = _owner;
	Corntainer::AddItem(script);

}

void ScriptContainer::RemoveAllItem()
{

}

void ScriptContainer::RemoveItem(PScriptBase object)
{

}

void ScriptContainer::RemoveItemsType(const std::string& type)
{

}

void ScriptContainer::RemoveScript(PScriptBase object)
{

}
