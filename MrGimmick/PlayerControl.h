#pragma once
#include "ScriptBase.h"
#include "InputAll.h"
#include "MoveScript.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(InputAll);

class PlayerControl : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;

	ScriptBase* Clone() const override;
private:
	SInputAll _moveLeft;
	SInputAll _moveRight;
	SInputAll _jump;

	MoveScript* _moveScript;

	REGISTER_START(PlayerControl);
};

