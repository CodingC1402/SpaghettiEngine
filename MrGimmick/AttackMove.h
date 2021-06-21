#pragma once
#include "ScriptBase.h"

class AttackMove : public ScriptBase
{
public:
	AttackMove(PScene owner, bool isDisabled);

	void Load(nlohmann::json& input);
	void OnUpdate() override;
protected:
	SGameObj _starPrefab;
};

