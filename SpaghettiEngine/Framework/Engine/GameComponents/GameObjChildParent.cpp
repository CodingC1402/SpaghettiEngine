#include "GameObj.h"
#include "ScriptBase.h"

//===========================================================================================================================//
#pragma region Parent_Function
void GameObj::RemoveParent()
{
	if (!parent)
		return;

	_owner->PromoteGameObjToRoot(this);
	parent->RemoveChild(this);


	parent = nullptr;
}
void GameObj::AddParent(const PGameObj& gameObj)
{
	if (parent == gameObj)
		return;

	if (parent)
		RemoveParent();

	parent = gameObj;

	parent->AddChild(this);
	_owner->DemoteGameObjFromRoot(this);

	_transform -= parent->GetWorldTransform();
	_rotation -= parent->GetWorldRotation();
	const Vector3 parentScale = parent->GetWorldScale();
	_scale.x /= parentScale.x;
	_scale.y /= parentScale.y;
	_scale.z /= parentScale.z;

	_isTransformChanged = true;
	_isRotationChanged = true;
	_isScaleChanged = true;
	ForceRecalculateMatrix();
}

void GameObj::AddParentWithoutCalculateLocal(const PGameObj& gameObj)
{
	if (parent)
		RemoveParentWithoutCalculateLocal();

	parent = gameObj;
	parent->AddChild(this);
	_owner->DemoteGameObjFromRoot(this);
	ForceRecalculateMatrix();
}

void GameObj::RemoveParentWithoutCalculateLocal()
{
	if (!parent)
		return;

	_owner->PromoteGameObjToRoot(this);
	parent->RemoveChild(this);
	parent = nullptr;
	ForceRecalculateMatrix();
}
#pragma endregion
//===========================================================================================================================//
#pragma region Child_Function
PGameObj GameObj::AddChild(const PGameObj& child)
{
	child->AddParent(this);
	_childrenPtr.push_back(child->GetSharedPtr());
	_children.push_back(child);
	return _children.back();
}

PGameObj GameObj::AddChildClone(const PGameObj& child)
{
	SGameObj cloneObject = child->Clone();
	return AddChild(cloneObject.get());
}

PGameObj GameObj::AddChild()
{
	auto newObj = _owner->CreateGameObject();

	auto result = AddChild(newObj.get());
	result->Load(defaultJson);
	return result;
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