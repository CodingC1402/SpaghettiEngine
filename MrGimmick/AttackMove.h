#pragma once
#include "ScriptBase.h"

class AttackMove
{
public:
	AttackMove(PScene owner, bool isDisabled);

	void Load(nlohmann::json& input);
protected:
	SGameObj _starPrefab;
};

