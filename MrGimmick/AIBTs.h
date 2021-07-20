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

	void SetGameObject(GameObj* obj) noexcept;
	[[nodiscard]] GameObj* GetGameObject() const noexcept;

	void SetDeltaTime(float time) noexcept;
	[[nodiscard]] float GetDeltaTime() const noexcept;

	[[nodiscard]] SBTs Clone() override;

	void MoveLeft();
	void MoveRight();
	void StopMove();

	void Jump();
	void StopJump();
private:
	MoveScript* _moveScript;
	GameObj* _owner;
	float _deltaTime;

	REGISTER_BEHAVIOR_TREE_START(AIBTs);
};

