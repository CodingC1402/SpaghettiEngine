#pragma once
#include "ScriptBase.h"
#include "HealthUIComponent.h"
#include "HealthScript.h"

class HealthBarScript : public ScriptBase
{
public:
	void OnEnabled() override;
	void OnDisabled() override;

	void OnStart() override;
	void OnLateUpdate() override;
	void Load(nlohmann::json& input) override;
	HealthScript* GetPlayerHealthScript() const;

	static HealthBarScript* GetInstance();
private:
	HealthUIComponent* _healthBar = nullptr;
	HealthScript* _healthScript = nullptr;

	static inline HealthBarScript* __instance = nullptr;
	REGISTER_START(HealthBarScript);
};

