#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class FeetScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void OnCollide(CollideEvent& e) override;
	PScriptBase Clone() const override;
protected:
	MoveScript* _moveScript = nullptr;
	bool _isGrounded		= false; // Because start on air
	bool _isLastGrounded	= false;
private:
	REGISTER_START(FeetScript);
};

