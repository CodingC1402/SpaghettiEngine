#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class HeadScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;
	void Load(nlohmann::json& input) override;

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

	Vector3 _center;
	float _width = 0;
	float _height = 0;
private:
	REGISTER_START(HeadScript);
};

