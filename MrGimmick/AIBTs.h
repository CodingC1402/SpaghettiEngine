#pragma once
#include "BTs.h"
#include "GameObj.h"
#include "Macros.h"

class MoveScript;
CLASS_FORWARD_DECLARATION(AIBTs);

class AIBTs : public BTs
{
public:
	void AssignMoveScript(MoveScript* script);
	[[nodiscard]] GameObj* GetGameObject() const noexcept;

	void MoveLeft();
	void MoveRight();
	void StopMove();

	void Jump();
	void StopJump();
private:
	MoveScript* _moveScript;
	GameObj* _owner;

	REGISTER_BEHAVIOR_TREE_START(AIBTs);
};

