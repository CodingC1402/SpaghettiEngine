#pragma once
#include "ScriptBase.h"

class AutoCallDestroy : public ScriptBase
{
public:
	void OnUpdate() override;
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;
private:
	float _delay = 0;

	REGISTER_START(AutoCallDestroy);
};

