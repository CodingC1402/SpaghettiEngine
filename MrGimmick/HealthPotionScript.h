#pragma once
#include "SpecialAttackScript.h"

class HealthPotionScript : public SpecialAttackScript
{
public:
	void OnStart() override;
private:
	REGISTER_START(HealthPotionScript);
};

