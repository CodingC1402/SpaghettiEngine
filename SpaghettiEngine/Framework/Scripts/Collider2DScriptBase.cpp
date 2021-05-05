#include "Collider2DScriptBase.h"
#include "Physic.h"

REGISTER_FINISH(Collider2DScriptBase);

Collider2DScriptBase::Collider2DScriptBase(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(Collider2DScriptBase);
}

void Collider2DScriptBase::OnStart()
{
	body = dynamic_cast<PRigidBody>(_ownerObj->GetScript("RigidBody"));
}
