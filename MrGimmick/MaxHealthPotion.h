#pragma once
#include "ScriptBase.h"

class MaxHealthPotion : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;
private:
	unsigned _healthIncrease = 0;

	REGISTER_START(MaxHealthPotion);
};

