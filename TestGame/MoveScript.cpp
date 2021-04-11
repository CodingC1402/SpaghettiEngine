#include "MoveScript.h"

REGISTER_FINISH(MoveScript);

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
		move.x += 1;
	if (right->Check())
		move.x -= 1;

	move.x *= movementSpeed;
	move.y *= movementSpeed;

	owner->Translate(move);
}
