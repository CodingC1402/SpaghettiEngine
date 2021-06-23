#include "ChildContainer.h"
#include "GameObj.h"

//===========================================================================================================================//

ChildContainer::ChildContainer(PGameObj owner)
{
	_owner = owner;
}

PGameObj ChildContainer::GetItem(const std::string& name)
{
	for (const auto& object : _container)
		if (object->GetName() == name)
			return object;
	return nullptr;
}

PGameObj ChildContainer::GetItemWithTag(const std::string& tag)
{
	for (const auto& object : _container)
		if (object->GetTag() == tag)
			return object;
	return nullptr;
}

std::deque<PGameObj> ChildContainer::GetAllItemsWithName(const std::string& name)
{
	std::deque<PGameObj> rValues;
	for (auto& object : _container)
		if (object->GetName() == name)
			rValues.push_back(object);
	return rValues;
}

std::deque<PGameObj> ChildContainer::GetAllItemsWithTag(const std::string& tag)
{
	std::deque<PGameObj> rValues;
	for (auto& object : _container)
		if (object->GetTag() == tag)
			rValues.push_back(object);
	return rValues;
}

SGameObj ChildContainer::AddItemClone(PGameObj child)
{
	SGameObj clone = child->Clone();
	AddItem(clone.get());
	return clone;
}

void ChildContainer::AddItem(PGameObj child)
{
	if (!child)
		return;

	if (child->GetParent())
		child->GetParent()->GetChildContainer().RemoveItem(child);

	child->_parent = _owner;
	_owner->GetTransform().AddChild(&child->GetTransform());
	Corntainer::AddItem(child);
}

void ChildContainer::RemoveAllItem()
{
	auto it = _container.begin();
	while (!_container.empty())
	{
		RemoveChild(*it);
		it = _container.erase(it);
	}
}

void ChildContainer::RemoveItem(PGameObj object)
{
	if (!object)
		return;

	for (auto it = _container.begin(); it != _container.end(); ++it)
	{
		if ((*it) == object)
		{
			RemoveChild(object);
			_container.erase(it);
			return;
		}
	}
}

void ChildContainer::RemoveItemsWithName(const std::string& name)
{
	for (auto it = _container.begin(); it != _container.end();)
	{
		if ((*it)->GetName() == name)
			it = _container.erase(it);
		else
			++it;
	}
}

void ChildContainer::RemoveItemsWithTag(const std::string& tag)
{
	for (auto it = _container.begin(); it != _container.end();)
	{
		if ((*it)->GetTag() == tag)
			it = _container.erase(it);
		else
			++it;
	}
}

void ChildContainer::RemoveChild(PGameObj object)
{
	_owner->GetTransform().RemoveChild(&object->GetTransform());
	object->_parent = nullptr;
}
