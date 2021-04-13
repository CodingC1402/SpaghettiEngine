#include "Rigidbody.h"
#include "GameTimer.h"

REGISTER_FINISH(RigidBody);

RigidBody::RigidBody()
{
	name = TYPE_NAME(RigidBody);
}

void RigidBody::Start()
{
	down = InputSystem::GetInput("MoveDown");
}

void RigidBody::Update()
{
	fall.x = 0;
	fall.y = -1;
	fall.z = 0;

	fall.y *= movementSpeed * GameTimer::GetDeltaTime();

	movementSpeed += gravity * GameTimer::GetDeltaTime();

	owner->Translate(&fall);
}
