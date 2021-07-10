#include "PhysicScriptBase.h"

PhysicScriptBase::PhysicScriptBase(PScene owner, bool isDisable)
	:
	ScriptBase(owner, isDisable)
{}

void PhysicScriptBase::OnDisabled()
{
	GetGameObject()->GetPhysicComponent().RemovePhysicComponent(this);
}

void PhysicScriptBase::OnEnabled()
{
	GetGameObject()->GetPhysicComponent().AddPhysicComponent(this);
}
