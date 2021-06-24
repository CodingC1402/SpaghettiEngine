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
	return nullptr;
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
	return nullptr;
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

	bool before = script->IsDisabled(); // Check before changing owner

	if (script->GetGameObject())
		script->GetGameObject()->GetScriptContainer().RemoveItem(script);
	script->_ownerObj = _owner;

	bool after = script->IsDisabled(); // Check after changing owner

	Add(script);
	script->SetContainerIterator(--_container.end());

	if (after != before)
	{
		if (after)
			script->OnDisabled();
		else
			script->OnEnabled();
	}
}

void ScriptContainer::RemoveAllItem()
{
	auto it = _container.begin();
	while (!_container.empty())
	{
		RemoveScript(*it);
		it = _container.erase(it);
	}
}

void ScriptContainer::RemoveItem(PScriptBase object)
{
	if (object->GetGameObject() != _owner)
		return;

	RemoveScript(object);
	Erase(object->GetContainerIterator());
}

void ScriptContainer::RemoveItemsType(const std::string& type)
{
	for (auto it = _container.begin(); it != _container.end();)
	{
		if ((*it)->GetType() == type)
		{
			RemoveScript(*it);
			it = _container.erase(it);
		}
		else
			++it;
	}
}

void ScriptContainer::RemoveScript(PScriptBase object)
{
	object->_ownerObj = nullptr;
}
