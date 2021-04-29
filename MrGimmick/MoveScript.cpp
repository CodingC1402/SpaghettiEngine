#include "MoveScript.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "ExMath.h"

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

	cam = Graphics::GetActiveCamera();
	cam->SetFollow(owner);
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
	{
		move.x -= 1;
		if (!isFlipped)
		{
			owner->SetScale(-1, 1, 1);
			isFlipped = true;
		}
	}
	if (right->Check())
	{
		move.x += 1;
		if (isFlipped)
		{
			owner->SetScale(1, 1, 1);
			isFlipped = false;
		}
	}

	move.x *= movementSpeed * GameTimer::GetDeltaTime();
	move.y *= movementSpeed * GameTimer::GetDeltaTime();

	owner->Translate(move);
}