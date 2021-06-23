#include "ChildContainer.h"
#include "GameObj.h"

//===========================================================================================================================//
#pragma region Parent_Function
void GameObj::RemoveParent()
{
	if (!parent)
		return;


}
void GameObj::AddParent(const PGameObj& gameObj)
{
	if (!gameObj)
		gameObj->AddChild(this);
}
#pragma endregion
//===========================================================================================================================//
#pragma region Child_Function
PGameObj GameObj::AddChild(const PGameObj& child)
{
	if (child->_parent == this || !child)
		return child;

	if (child->_parent)
		child->_parent->RemoveChild(child);

	_transform.AddChild(&child->GetTransform());
}

PGameObj GameObj::AddChildClone(const PGameObj& child)
{

}

PGameObj GameObj::AddChild()
{

}
#pragma endregion
//===========================================================================================================================//
#pragma region ParentChild Getters
PGameObj GameObj::GetParent() const
{
	return parent;
}
PGameObj GameObj::GetChild(unsigned index) const
{
	if (index >= _children.size())
		return nullptr;

	auto iterator = _children.begin();
	std::advance(iterator, index);
	return  *iterator;
}
#pragma endregion

PGameObj ChildContainer::GetGameObject(const std::string& name)
{
	for (const auto& object : _container)
		object->
}
