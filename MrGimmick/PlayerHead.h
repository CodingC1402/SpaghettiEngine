#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class PlayerHead : public ScriptBase
{
public:
	PlayerHead(PScene owner, bool isDisabled = true);

	void OnStart() override;
	void OnFixedUpdate() override;
	void OnCollide(CollideEvent& e) override;

	PScriptBase Clone() const override;
protected:
	//
	// this is to make sure that you can't jump if there is an obsticle over your head
	// |            +---+ |
	// +------------|---|-+
	//   +---+      +---+
	//   |   |
	//   +---+    Doesn't allow
	// 
	// 	Allow Jump
	//
	bool _isCollideWithPlatform = false;
	MoveScript* _moveScript;
private:
	REGISTER_START(PlayerHead);
};

