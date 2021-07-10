#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class HeadScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;
	void OnCollide(CollideEvent& e) override;

	PScriptBase Clone() const override;
protected:
	//
	// this is to make sure that you can't jump if there is an obsticle over your head
	// |            +---+ |
	// +------------|-H-|-+
	//   +---+      +---+
	//   | H |
	//   +---+    Doesn't allow
	// 
	// 	Allow Jump
	//
	bool _isCollideWithPlatform = false;
	MoveScript* _moveScript = nullptr;
private:
	REGISTER_START(HeadScript);
};

