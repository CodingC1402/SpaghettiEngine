#pragma once
#include "ScriptBase.h"
#include "InputAll.h"
#include "MoveScript.h"
#include "Macros.h"
#include "Animator.h"

CLASS_FORWARD_DECLARATION(InputAll);

class PlayerControl : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;

	void OnDisabled() override;
	void OnEnabled() override;
	void SetIsInTube(bool value);

	ScriptBase* Clone() const override;
private:
	SInputAll _moveLeft;
	SInputAll _moveRight;
	SInputAll _jump;

	BoolField _isInTube;
	MoveScript* _moveScript = nullptr;

	REGISTER_START(PlayerControl);
};

