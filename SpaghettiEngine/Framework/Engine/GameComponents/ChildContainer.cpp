#include "ChildContainer.h"
#include "GameObj.h"

//===========================================================================================================================//

ChildContainer::ChildContainer(PGameObj owner)
{
	_owner = owner;
}

PGameObj& ChildContainer::operator[](unsigned index)
{
	return _container[index];
}

unsigned ChildContainer::GetSize()
{
	return _container.size();
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

void ChildContainer::AddItem(PGameObj child)
{

}

void ChildContainer::RemoveAllItem()
{
	Corntainer::RemoveAllItem();
}

void ChildContainer::RemoveItem(PGameObj object)
{
	Corntainer::RemoveItem(object);
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
