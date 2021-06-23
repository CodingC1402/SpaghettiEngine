#pragma once
#include "ScriptBase.h"
#include "InputSystem.h"

class AttackMove : public ScriptBase
{
public:
	AttackMove(PScene owner, bool isDisabled = false);

	void OnStart() override;
	void OnUpdate() override;
	void Load(nlohmann::json& input);
protected:
	PGameObj _starPrefab;
	SInput _attackKey;
private:
	REGISTER_START(AttackMove);
};

