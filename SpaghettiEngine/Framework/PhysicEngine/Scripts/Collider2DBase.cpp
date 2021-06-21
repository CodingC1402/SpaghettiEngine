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

	PhysicComponent* physic = &_ownerObj->GetPhysicComponent();
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

	_ownerObj->GetPhysicComponent().UnSubscribeTo2D(this);
	for (auto& shape : _shapes)
		Physic::RemoveShape(shape.get());

	if constexpr (Setting::IsDebugMode())
		for (auto& linerender : _lineRenderer)
			linerender->OnDisabled();
}

void Collider2DBase::OnChange()
{
	ChangeBody(_ownerObj->GetPhysicComponent().GetBody2D());
}

void Collider2DBase::AssignOwner(const PGameObj& gameObj)
{
	PhysicScriptBase::AssignOwner(gameObj);
	if constexpr(Setting::IsDebugMode())
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

}

SScriptBase Collider2DBase::Clone() const
{
	auto clone = std::dynamic_pointer_cast<Collider2DBase>(PhysicScriptBase::Clone());

	if constexpr (Setting::IsDebugMode())
	{
		for (const auto& lineRenderer : _lineRenderer)
			clone->_lineRenderer.push_back(std::dynamic_pointer_cast<LineRendererBase>(lineRenderer->Clone()));
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
		linerender->AssignOwner(_ownerObj);
}

void Collider2DBase::ChangeBody(WBody2D body)
{
	_body = body;
	for (auto& shape : _shapes)
		shape->SetBody(_body);
}
