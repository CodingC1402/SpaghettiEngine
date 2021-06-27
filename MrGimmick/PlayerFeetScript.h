#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class PlayerFeetScript : public ScriptBase
{
public:
	PlayerFeetScript(PScene owner, bool isDisabled = false);

	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;

	void OnCollide(CollideEvent& e) override;
	void OnCollideEnter(CollideEvent& e) override;
	void OnCollideExit(CollideEvent& e) override;
	PScriptBase Clone() const override;
protected:
	MoveScript* _moveScript = nullptr;
	bool _isGrounded		= false; // Because start on air
	bool _isLastGrounded	= false;
private:
	REGISTER_START(PlayerFeetScript);
};

