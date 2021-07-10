#include "RigidBody2D.h"
#include "PhysicComponent.h"
#include "Physic.h"
#include "MaterialContainer.h"

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
	if (input[_velocityField] != nullptr)
	{
		Vector3 velocity(input[_velocityField][0], input[_velocityField][1], input[_velocityField][2]);
		_body->SetVelocity(velocity);
	}

	if (input[_massField] != nullptr)
		_body->SetMass(input[_massField].get<float>());
	if (input[_gravityScale] != nullptr)
		_body->SetGravityScale(input[_gravityScale].get<float>());
	if (input[_materialField] != nullptr)
		_body->SetMaterial(MaterialContainer::GetInstance()->GetResource(input[_materialField].get<CULL>()));
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
