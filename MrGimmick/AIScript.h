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
	SAIBTs GetBTS();
private:
	float _delay = 0.0f;
	float _accumulate = 0.0f;

	SAIBTs _behaviorTree;
	MoveScript* _moveScript = nullptr;

	REGISTER_START(AIScript);
};

