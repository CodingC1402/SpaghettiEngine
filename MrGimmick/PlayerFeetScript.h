#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class PlayerFeetScript : public ScriptBase
{
public:
	void OnStart() override;

	void OnCollideEnter(CollideEvent& e) override;
	void OnCollideExit(CollideEvent& e) override;
	PScriptBase Clone() const override;
protected:
	MoveScript* _moveScript;
	bool _isGrounded = false; // Because start on air
};

