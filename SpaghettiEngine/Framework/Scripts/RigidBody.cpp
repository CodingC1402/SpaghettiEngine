#include "Rigidbody.h"
#include "GameTimer.h"

REGISTER_FINISH(RigidBody);

RigidBody::RigidBody()
{
	name = TYPE_NAME(RigidBody);
}

void RigidBody::Start()
{
}

void RigidBody::Update()
{
	fall.x = 0;
	fall.y = -1;
	fall.z = 0;

	fall.y *= vy * GameTimer::GetDeltaTime();

	vy += gravity * GameTimer::GetDeltaTime();

	//owner->Translate(&fall);
}

void RigidBody::Load(const std::string* inputArg)
{
	try
	{
		Physic::GetInstance()->AddRigidBody(this);
	}
	catch (CornException& e)
	{
		throw e;
	}
}
