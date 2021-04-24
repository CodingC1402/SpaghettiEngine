#include "MoveScript.h"
#include "GameTimer.h"
#include "Debug.h"

REGISTER_FINISH(MoveScript);

MoveScript::MoveScript()
{
	name = TYPE_NAME(MoveScript);
}

void MoveScript::Start()
{
	up = InputSystem::GetInput("MoveUp");
	down = InputSystem::GetInput("MoveDown");
	left = InputSystem::GetInput("MoveLeft");
	right = InputSystem::GetInput("MoveRight");
}

void MoveScript::Update()
{
	move.x = 0;
	move.y = 0;
	move.z = 0;

	if (up->Check())
		move.y += 1;
	if (down->Check())
		move.y -= 1;
	if (left->Check())
		move.x -= 1;
	if (right->Check())
		move.x += 1;

	move.x *= movementSpeed * GameTimer::GetDeltaTime();
	move.y *= movementSpeed * GameTimer::GetDeltaTime();

	owner->Translate(&move);
	std::static_pointer_cast<RigidBody>(owner->GetScript("RigidBody")).get()->UpdateVelocity(move);
}
