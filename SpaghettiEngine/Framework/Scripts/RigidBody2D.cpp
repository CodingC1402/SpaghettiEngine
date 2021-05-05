#include "Rigidbody2D.h"
#include "GameTimer.h"

REGISTER_FINISH(RigidBody2D);

RigidBody2D::RigidBody2D(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(RigidBody2D);
}

void RigidBody2D::OnStart()
{
}

void RigidBody2D::OnUpdate()
{
	//fall.x = 0;
	//fall.y = -1;
	//fall.z = 0;
	//
	//fall.y *= vy * GameTimer::GetDeltaTime();
	//vy += gravity * GameTimer::GetDeltaTime();
	//
	//owner->Translate(&fall);
}

void RigidBody2D::SetVelocity(Vector3 v)
{
	this->vx = v.x;
	this->vy = v.y;
}

void RigidBody2D::SetVelocityByDelta(Vector3 delta)
{
	this->vx += delta.x;
	this->vy += delta.y;
}

Vector3 RigidBody2D::GetVelocity()
{
	return Vector3(vx, vy, 1);
}

void RigidBody2D::Load(json& inputObject)
{
	try
	{
		//Physic::GetInstance()->AddRigidBody(this);
	}
	catch (CornException& e)
	{
		throw e;
	}
}