#pragma once
#include "ScriptBase.h"
#include "AttackScript.h"

class SpecialAttackScript : public ScriptBase
{
public:
	void OnStart() override;
	AttackScript* GetAttackScript() const noexcept;
private:
	AttackScript* _attackScript = nullptr;

	REGISTER_START(SpecialAttackScript);
};

