#include "Rigidbody.h"
#include "GameTimer.h"

REGISTER_FINISH(Rigidbody);

Rigidbody::Rigidbody()
{
	name = TYPE_NAME(Rigidbody);
}

void Rigidbody::Update()
{
	fall.x *= movementSpeed * GameTimer::GetDeltaTime();
	movementSpeed += gravity * GameTimer::GetDeltaTime() * GameTimer::GetDeltaTime() / 2;
	owner->Translate(&fall);
}
