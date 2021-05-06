#include "Collider2DScriptBase.h"
#include "Physic.h"

REGISTER_FINISH(Collider2DScriptBase);

Collider2DScriptBase::Collider2DScriptBase(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(Collider2DScriptBase);
}

void Collider2DScriptBase::OnEnabled()
{
	Physic::GetInstance()->AddCollider(this);
}

void Collider2DScriptBase::OnDisabled()
{
	Physic::GetInstance()->RemoveCollider(this);
}

void Collider2DScriptBase::OnStart()
{
	body = dynamic_cast<PRigidBody2D>(_ownerObj->GetScript("RigidBody"));
}

Vector3 Collider2DScriptBase::GetPosition()
{
	return _ownerObj->GetWorldTransform();
}

void Collider2DScriptBase::SetTrigger(bool boolean)
{
	isTrigger = boolean;
}

void Collider2DScriptBase::SetOffSet(Vector3 offset)
{
	offSet = offset;
}
