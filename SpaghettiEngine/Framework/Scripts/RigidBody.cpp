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
	fall.y = 0;
	fall.z = 0;

	if (down->Check())
		fall.y -= 1;

	fall.x *= movementSpeed * GameTimer::GetDeltaTime();
	owner->Translate(&fall);
}
