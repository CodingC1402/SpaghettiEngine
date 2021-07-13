#pragma once
#include "ScriptBase.h"

class HealthScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input);
	void OnStart() override;
	void OnCollide(CollideEvent& e) override;

	PScriptBase Clone() const override;
protected:
	int currentHealth;
	int maxHealth;
private:
	REGISTER_START(HealthScript);
};