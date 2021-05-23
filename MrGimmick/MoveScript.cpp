#include "MoveScript.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "ExMath.h"

REGISTER_FINISH(MoveScript);

MoveScript::MoveScript(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(MoveScript);
}

void MoveScript::OnStart()
{
	up = InputSystem::GetInput("MoveUp");
	down = InputSystem::GetInput("MoveDown");
	left = InputSystem::GetInput("MoveLeft");
	right = InputSystem::GetInput("MoveRight");

	body = dynamic_cast<PRigidBody2D>(_ownerObj->GetScript("RigidBody2D"));
}

void MoveScript::OnUpdate()
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
			_ownerObj->SetScale(-1, 1, 1);
			isFlipped = true;
		}
	}
	if (right->Check())
	{
		move.x += 1;
		if (isFlipped)
		{
			_ownerObj->SetScale(1, 1, 1);
			isFlipped = false;
		}
	}

	move.x *= movementSpeed * GameTimer::GetDeltaTime();
	move.y *= movementSpeed * GameTimer::GetDeltaTime();

	body->AddForce(move);
	_ownerObj->Translate(move);
}