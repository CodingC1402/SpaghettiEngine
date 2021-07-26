#pragma once
#include "SpecialAttackScript.h"

class BombScript : public SpecialAttackScript
{
public:
	void OnStart() override;
private:
	REGISTER_START(BombScript);
};

