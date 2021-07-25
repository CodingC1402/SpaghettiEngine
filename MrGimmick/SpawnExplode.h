#pragma once
#include "ScriptBase.h"

class SpawnExplode : public ScriptBase
{
public:
	void OnStart() override;
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;
private:
	GameObj* _prefab = nullptr;
	bool _stop = false;

	REGISTER_START(SpawnExplode);
};

