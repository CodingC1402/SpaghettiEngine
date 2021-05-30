#include "PhysicScriptBase.h"

PhysicScriptBase::PhysicScriptBase(PScene owner, bool isDisable)
	:
	ScriptBase(owner, isDisable)
{}

void PhysicScriptBase::AssignOwner(const PGameObj& owner)
{
	ScriptBase::AssignOwner(owner);
	owner->AddPhysicComponent(this);
}