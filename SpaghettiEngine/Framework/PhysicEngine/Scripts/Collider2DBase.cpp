#include "Collider2DBase.h"
#include "Physic.h"

Collider2DBase::Collider2DBase(PScene owner) 
	:
	ScriptBase(owner)
{}

void Collider2DBase::OnEnabled()
{
	PhysicComponent* physic = &_ownerObj->GetPhysicComponent();
	physic->SubscribeTo2D(this);
	ChangeBody(physic->GetBody2D());
}

void Collider2DBase::OnDisabled()
{
	_ownerObj->GetPhysicComponent().UnSubscribeTo2D(this);
}

void Collider2DBase::OnChange()
{
	ChangeBody(_ownerObj->GetPhysicComponent().GetBody2D());
}

void Collider2DBase::Load(nlohmann::json& input)
{
	if (input[_offSetXField] != nullptr)
		_shape->SetOffSetX(input[_offSetXField]);
	if (input[_offSetYField] != nullptr)
		_shape->SetOffSetY(input[_offSetYField]);
}

void Collider2DBase::ChangeBody(WBody2D body)
{
	_body = body;
	_shape->SetBody(_body);
}
