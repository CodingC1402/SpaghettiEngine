#include "Collider2DBase.h"
#include "Physic.h"
#include "Setting.h"

Collider2DBase::Collider2DBase(PScene owner) 
	:
	ScriptBase(owner)
{
	if constexpr (Setting::IsDebugMode())
		_lineRenderer = dynamic_cast<LineRendererBase*>(ScriptFactory::CreateInstance("LineRendererBase", _owner));
}

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

void Collider2DBase::AssignOwner(const PGameObj& gameObj)
{
	ScriptBase::AssignOwner(gameObj);
	SetLineRendererOwner();
}

void Collider2DBase::SetLineRendererOwner()
{
	if constexpr (Setting::IsDebugMode())
	{
		_lineRenderer->_ownerObj = _ownerObj;
	}
}

void Collider2DBase::Load(nlohmann::json& input)
{
	if (input[_offSetXField] != nullptr)
		_shape->SetOffSetX(input[_offSetXField]);
	if (input[_offSetYField] != nullptr)
		_shape->SetOffSetY(input[_offSetYField]);
}

Collider2DBase::~Collider2DBase()
{
	delete _lineRenderer;
	_lineRenderer = nullptr;
}

void Collider2DBase::ChangeBody(WBody2D body)
{
	_body = body;
	_shape->SetBody(_body);
}
