#pragma once
#include "SpecialAttackScript.h"
#include "AttackScript.h"

class FireBallScript : public SpecialAttackScript
{
public:
	void OnStart();
	void OnCollide(CollideEvent& e) override;
private:
	AttackScript* _script = nullptr;

	REGISTER_START(FireBallScript);
};

