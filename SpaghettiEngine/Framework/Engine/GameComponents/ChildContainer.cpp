#include "ChildContainer.h"
#include "GameObj.h"

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

std::list<PGameObj> ChildContainer::GetAllItemsWithName(const std::string& name)
{
	std::list<PGameObj> rValues;
	for (auto& object : _container)
		if (object->GetName() == name)
			rValues.push_back(object);
	return rValues;
}

std::list<PGameObj> ChildContainer::GetAllItemsWithTag(const std::string& tag)
{
	std::list<PGameObj> rValues;
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

	// If it's parent is null then it could be a root object if owner is not null
	// Then it's a root object.
	if (child->GetParent())
	{
		auto& container = child->GetParent()->GetChildContainer();
		container.RemoveChildWithoutEvent(child);
		container._container.erase(child->GetContainerIterator());
	}
	else if (child->GetOwner())
		child->GetOwner()->RemoveFromRoot(child);

	// Disability and then call event accordingly
	_owner->GetTransform().AddChild(&child->GetTransform());
	child->SetParentInternally(child);

	Corntainer::AddItem(child);
	child->SetContainerIterator(--_container.end());
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
		{
			RemoveChild(*it);
			it = _container.erase(it);
		}
		else
			++it;
	}
}

void ChildContainer::RemoveItemsWithTag(const std::string& tag)
{
	for (auto it = _container.begin(); it != _container.end();)
	{
		if ((*it)->GetTag() == tag)
		{
			RemoveChild(*it);
			it = _container.erase(it);
		}
		else
			++it;
	}
}

void ChildContainer::RemoveChild(PGameObj object)
{
	bool childBefore = object->IsDisabled();
	RemoveChildWithoutEvent(object);
	if (childBefore && !object->IsDisabled())
		object->OnEnabled();

	if (object->GetOwner())
		object->GetOwner()->AddToRoot(object);
}

void ChildContainer::RemoveChildWithoutEvent(PGameObj object)
{
	_owner->GetTransform().RemoveChild(&object->GetTransform());
	object->_parent = nullptr;
}
