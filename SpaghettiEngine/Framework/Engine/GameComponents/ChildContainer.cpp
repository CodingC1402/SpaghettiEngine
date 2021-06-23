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

PGameObj ChildContainer::AddItemClone(PGameObj child)
{
	PGameObj clone = child->Clone();
	AddItem(clone);
	return clone;
}

void ChildContainer::AddItem(PGameObj child)
{
	if (!child)
		return;

	// If it's parent is null then it could be a root object if owner is not null
	// Then it's a root object.
	if (child->GetParent())
		child->GetParent()->GetChildContainer().RemoveChildWithoutEvent(child);
	else if (child->GetOwner())
		child->GetOwner()->RemoveFromRoot(child);

	_owner->GetTransform().AddChild(&child->GetTransform());

	Corntainer::AddItem(child);
	child->SetContainerIterator(--_container.end());

	child->SetParentInternally(child);
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
	if (!object || object->GetParent() != _owner)
		return;

	RemoveChild(object);
	_container.erase(object->GetContainerIterator());
	return;
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
	RemoveChildFromTransform(object);
	object->_parent = nullptr;
	if (childBefore && !object->IsDisabled())
		object->OnEnabled();

	if (object->GetOwner())
		object->GetOwner()->AddToRoot(object);
}

void ChildContainer::RemoveChildFromTransform(PGameObj object)
{
	_owner->GetTransform().RemoveChild(&object->GetTransform());
}

void ChildContainer::RemoveChildWithoutEvent(PGameObj object)
{
	RemoveChildFromTransform(object);
	_container.erase(object->GetContainerIterator());
}
