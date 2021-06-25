#include "Collider2DBase.h"
#include "Physic.h"
#include "Setting.h"
#include "LineRenderBase.h"

Collider2DBase::Collider2DBase(PScene owner, bool isDisable) 
	:
	PhysicScriptBase(owner, isDisable)
{}

void Collider2DBase::OnEnabled()
{
	PhysicScriptBase::OnEnabled();

	PhysicComponent* physic = &GetGameObject()->GetPhysicComponent();
	physic->SubscribeTo2D(this);
	ChangeBody(physic->GetBody2D());

	for (auto& shape : _shapes)
		Physic::AddShape(shape.get());

	if constexpr (Setting::IsDebugMode())
		for (auto& linerender : _lineRenderer)
			linerender->OnEnabled();
}

void Collider2DBase::OnDisabled()
{
	PhysicScriptBase::OnDisabled();

	GetGameObject()->GetPhysicComponent().UnSubscribeTo2D(this);

	for (auto& shape : _shapes)
		Physic::RemoveShape(shape.get());

	if constexpr (Setting::IsDebugMode())
		for (auto& linerender : _lineRenderer)
			linerender->OnDisabled();
}

void Collider2DBase::OnChange()
{
	ChangeBody(GetGameObject()->GetPhysicComponent().GetBody2D());
}

void Collider2DBase::SetGameObject(const PGameObj& gameObj)
{
	PhysicScriptBase::SetGameObject(gameObj);
	if constexpr (Setting::IsDebugMode())
		SetLineRendererOwner();
}

void Collider2DBase::Load(nlohmann::json& input)
{
	float offSet = 0;

	if (input[_offSetXField] != nullptr)
	{
		offSet = input[_offSetXField];

		if constexpr (Setting::IsDebugMode())
			for (auto& linerender : _lineRenderer)
				linerender->SetOffSetX(offSet);

		for (auto& shape : _shapes)
			shape->SetOffSetX(offSet);
	}
	if (input[_offSetYField] != nullptr)
	{
		offSet = input[_offSetYField];

		if constexpr (Setting::IsDebugMode())
			for (auto& linerender : _lineRenderer)
				linerender->SetOffSetY(offSet);

		for (auto& shape : _shapes)
			shape->SetOffSetY(offSet);
	}

	if (!input[_isTriggerField].empty())
		SetIsTrigger(input[_isTriggerField].get<bool>());

	SetOwnerForShapes();
}

bool Collider2DBase::CallDestroy()
{
	auto destroy = ScriptBase::CallDestroy();
	if (!destroy)
		return false;

	for (auto it = _lineRenderer.begin(); it != _lineRenderer.end(); ++it)
		(*it)->CallDestroy();
	_lineRenderer.clear();
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

	if constexpr (Setting::IsDebugMode())
	{
		for (const auto& lineRenderer : _lineRenderer)
			clone->_lineRenderer.push_back(dynamic_cast<LineRendererBase*>(lineRenderer->Clone()));
	}

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
	_lineRenderer.clear();
}

void Collider2DBase::SetLineRendererOwner()
{
	for (auto& linerender : _lineRenderer)
		linerender->SetGameObject(GetGameObject());
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
