#include "PhysicScriptBase.h"

PhysicScriptBase::PhysicScriptBase(PScene owner, bool isDisable)
	:
	ScriptBase(owner, isDisable)
{}

void PhysicScriptBase::AssignOwner(const PGameObj& owner)
{
	ScriptBase::AssignOwner(owner);
}

void PhysicScriptBase::OnDisabled()
{
	_ownerObj->RemovePhysicComponent(this);
}

void PhysicScriptBase::OnEnabled()
{
	_ownerObj->AddPhysicComponent(this);
}
