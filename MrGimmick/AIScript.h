#pragma once
#include "ScriptBase.h"
#include "AIBTs.h"

class AIScript : public ScriptBase
{
public:
	void OnFixedUpdate() override;
	void Load(nlohmann::json&) override;
private:
	float _delay;
	float _accumulate;

	SAIBTs _behaviralTree;

	REGISTER_START(AIScript);
};

