#include "RigidBody2D.h"
#include "PhysicComponent.h"
#include "Physic.h"

REGISTER_FINISH(RigidBody2D);

RigidBody2D::RigidBody2D(PScene owner, bool isDisabled) : PhysicScriptBase(owner, isDisabled)
{
	_name = TYPE_NAME(RigidBody2D);
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
}
