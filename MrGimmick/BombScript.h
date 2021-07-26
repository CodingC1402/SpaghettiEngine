#pragma once
#include "SpecialAttackScript.h"

class BombScript : public SpecialAttackScript
{
public:
	void OnStart() override;
	void Explode();
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;
private:
	GameObj* _prefab = nullptr;

	REGISTER_START(BombScript);
};

