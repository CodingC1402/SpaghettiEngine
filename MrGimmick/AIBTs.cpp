#include "AIBTs.h"
#include "MoveScript.h"

REGISTER_BEHAVIOR_TREE_FINISH(AIBTs);

void AIBTs::AssignMoveScript(MoveScript* script)
{
	_moveScript = script;
}

void AIBTs::SetGameObject(GameObj* obj) noexcept
{
	_owner = obj;
}

GameObj* AIBTs::GetGameObject() const noexcept
{
	return _owner;
}

void AIBTs::SetDeltaTime(float time) noexcept
{
	_deltaTime = time;
}

float AIBTs::GetDeltaTime() const noexcept
{
	return _deltaTime;
}

void AIBTs::MoveLeft()
{
	_moveScript->StopMoveRight();
	_moveScript->StartMoveLeft();
}

void AIBTs::MoveRight()
{
	_moveScript->StopMoveLeft();
	_moveScript->StartMoveRight();
}

void AIBTs::StopMove()
{
	_moveScript->StopMoveRight();
	_moveScript->StopMoveLeft();
}

void AIBTs::Jump()
{
	_moveScript->StopJump();
	_moveScript->StartJump();
}

void AIBTs::StopJump()
{
	_moveScript->StopJump();
}
