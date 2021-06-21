#include "RigidBody2D.h"
#include "PhysicComponent.h"
#include "Physic.h"
#include "MaterialContainer.h"

REGISTER_FINISH(RigidBody2D);

RigidBody2D::RigidBody2D(PScene owner, bool isDisabled) : PhysicScriptBase(owner, isDisabled)
{
	_body = std::make_shared<Body2D>();
}

void RigidBody2D::OnDisabled()
{
	PhysicScriptBase::OnDisabled();

	_ownerObj->GetPhysicComponent().Remove2DBody(_body);
	Physic::RemoveRigid2DScript(this);
	Physic::RemoveBody(_body.get());
}

void RigidBody2D::OnEnabled()
{
	PhysicScriptBase::OnEnabled();

	_ownerObj->GetPhysicComponent().Set2DBody(_body);
	Physic::AddRigid2DScript(this);
	Physic::AddBody(_body.get());
}

void RigidBody2D::AfterPhysicUpdate()
{
	auto vec = _body->GetPosition() - GetWorldTransform();
	_ownerObj->Translate(vec);
}

SScriptBase RigidBody2D::Clone() const
{
	auto clone = std::dynamic_pointer_cast<RigidBody2D>(ScriptBase::Clone());
	
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
