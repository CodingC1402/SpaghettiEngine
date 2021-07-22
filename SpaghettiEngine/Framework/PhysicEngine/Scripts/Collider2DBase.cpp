#include "Collider2DBase.h"
#include "Physic.h"
#include "Setting.h"
#include "ScriptField.h"

Collider2DBase::Collider2DBase(PScene owner, bool isDisable) 
	:
	PhysicScriptBase(owner, isDisable)
{}

void Collider2DBase::OnStart()
{
	for (auto& shape : _shapes)
		shape->SetOwnerScript(this);
}

void Collider2DBase::OnEnabled()
{
	PhysicScriptBase::OnEnabled();

	PhysicComponent* physic = &GetGameObject()->GetPhysicComponent();
	physic->SubscribeTo2D(this);
	ChangeBody(physic->GetBody2D());

	for (auto& shape : _shapes)
		Physic::AddShape(shape.get());
}

void Collider2DBase::OnDisabled()
{
	PhysicScriptBase::OnDisabled();

	GetGameObject()->GetPhysicComponent().UnSubscribeTo2D(this);

	for (auto& shape : _shapes)
		Physic::RemoveShape(shape.get());
}

void Collider2DBase::OnChange()
{
	ChangeBody(GetGameObject()->GetPhysicComponent().GetBody2D());
}

void Collider2DBase::SetGameObject(const PGameObj& gameObj)
{
	PhysicScriptBase::SetGameObject(gameObj);
}

void Collider2DBase::Load(nlohmann::json& input)
{
	using Fields::Collider2DBase;
	float offSet = 0;

	if (input[Collider2DBase::GetOffSetXField()] != nullptr)
	{
		offSet = input[Collider2DBase::GetOffSetXField()];

		for (auto& shape : _shapes)
			shape->SetOffSetX(offSet);
	}
	if (input[Collider2DBase::GetOffSetYField()] != nullptr)
	{
		offSet = input[Collider2DBase::GetOffSetYField()];

		for (auto& shape : _shapes)
			shape->SetOffSetY(offSet);
	}

	if (!input[Collider2DBase::GetTriggerField()].empty())
		SetIsTrigger(input[Collider2DBase::GetTriggerField()].get<bool>());

	SetOwnerForShapes();
}

bool Collider2DBase::CallDestroy()
{
	auto destroy = ScriptBase::CallDestroy();
	if (!destroy)
		return false;

	return true;
}

void Collider2DBase::SetIsTrigger(bool value)
{
	if (_isTrigger == value)
		return;

	_isTrigger = value;
	for (auto& shape : _shapes)
		shape->SetTriggerOnly(_isTrigger);
}

bool Collider2DBase::IsTrigger() const
{
	return _isTrigger;
}

PScriptBase Collider2DBase::Clone() const
{
	auto clone = dynamic_cast<Collider2DBase*>(PhysicScriptBase::Clone());

	SShape clonedShape;
	for (const auto& shape : _shapes)
	{
		clonedShape.reset(shape->Clone());
		clone->_shapes.push_back(clonedShape);
	}

	return clone;
}

Collider2DBase::~Collider2DBase()
{
}

void Collider2DBase::ChangeBody(WBody2D body)
{
	_body = body;
	for (auto& shape : _shapes)
		shape->SetBody(_body);
}

void Collider2DBase::SetOwnerForShapes()
{
	for (auto& shape : _shapes)
		shape->SetOwnerScript(this);
}
