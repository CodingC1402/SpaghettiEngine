#pragma once
#include "ScriptBase.h"
#include "HealthUIComponent.h"
#include "HealthScript.h"

class HealthBarScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnLateUpdate() override;
	void Load(nlohmann::json& input) override;
private:
	HealthUIComponent* _healthBar = nullptr;
	HealthScript* _healthScript = nullptr;

	REGISTER_START(HealthBarScript);
};

