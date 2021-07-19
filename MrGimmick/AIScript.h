#pragma once
#include "ScriptBase.h"
#include "AIBTs.h"

class AIScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;
	void Load(nlohmann::json&) override;
	ScriptBase* Clone() const override;
private:
	float _delay;
	float _accumulate;

	SAIBTs _behaviorTree;
	MoveScript* _moveScript;

	REGISTER_START(AIScript);
};

