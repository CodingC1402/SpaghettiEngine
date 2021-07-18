#include "AIBTs.h"
#include "MoveScript.h"

REGISTER_BEHAVIOR_TREE_FINISH(AIBTs);

void AIBTs::AssignMoveScript(MoveScript* script)
{
	_moveScript = script;
	_owner = _moveScript->GetGameObject();
}

GameObj* AIBTs::GetGameObject() const noexcept
{
	return _owner;
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
