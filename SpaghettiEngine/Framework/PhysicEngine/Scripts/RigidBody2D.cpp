#include "MaterialContainer.h"
#include "PhysicComponent.h"
#include "Physic.h"
#include "RigidBody2D.h"
#include "ScriptField.h"

REGISTER_FINISH(RigidBody2D, PhysicScriptBase)
{
	_body = std::make_shared<Body2D>();
}

void RigidBody2D::OnDisabled()
{
	PhysicScriptBase::OnDisabled();

	GetGameObject()->GetPhysicComponent().Remove2DBody(this);
	Physic::RemoveRigid2DScript(this);
	Physic::RemoveBody(_body.get());

	_body->SetVelocity(Vector3(0, 0, 0));
}

void RigidBody2D::OnEnabled()
{
	PhysicScriptBase::OnEnabled();

	GetGameObject()->GetPhysicComponent().Set2DBody(this);
	Physic::AddRigid2DScript(this);
	Physic::AddBody(_body.get());
}

void RigidBody2D::AfterPhysicUpdate()
{
	auto vec = _body->GetPosition() - GetWorldTransform();
	GetGameObject()->GetTransform().Translate(vec);
}

PScriptBase RigidBody2D::Clone() const
{
	auto clone = dynamic_cast<RigidBody2D*>(ScriptBase::Clone());
	
	clone->_body.reset(_body->Clone());

	return clone;
}

void RigidBody2D::Load(nlohmann::json& input)
{
	using Fields::RigidBody2D;

	if (input[RigidBody2D::GetVelocityField()] != nullptr)
	{
		Vector3 velocity(input[RigidBody2D::GetVelocityField()][0], input[RigidBody2D::GetVelocityField()][1], input[RigidBody2D::GetVelocityField()][2]);
		_body->SetVelocity(velocity);
	}

	if (input[RigidBody2D::GetMassField()] != nullptr)
		_body->SetMass(input[RigidBody2D::GetMassField()].get<float>());
	if (input[RigidBody2D::GetGravityScaleField()] != nullptr)
		_body->SetGravityScale(input[RigidBody2D::GetGravityScaleField()].get<float>());
	if (input[RigidBody2D::GetMaterialField()] != nullptr)
		_body->SetMaterial(MaterialContainer::GetInstance()->GetResource(input[RigidBody2D::GetMaterialField()].get<CULL>()));
}

SBody2D RigidBody2D::GetBody()
{
	return _body;
}

void RigidBody2D::SetVelocity(const Vector3& velocity)
{
	_body->SetVelocity(velocity);
}

const Vector3& RigidBody2D::GetVelocity() const
{
	return _body->GetVelocity();
}

void RigidBody2D::SetMass(const float& mass)
{
	_body->SetMass(mass);
}

const float& RigidBody2D::GetMass() const
{
	return _body->GetMass();
}

void RigidBody2D::SetGravityScale(const float& scalar)
{
	_body->SetGravityScale(scalar);
}

const float& RigidBody2D::GetGravityScale() const
{
	return _body->GetGravityScale();
}

void RigidBody2D::ApplyImpulseForce(const Vector3& force)
{
	_body->ApplyImpulse(force);
}
