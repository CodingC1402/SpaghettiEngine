#pragma once
#include "ScriptBase.h"
#include <map>

class LootTable : public ScriptBase
{
public:
	void Load(nlohmann::json& input) override;

	void OnDisabled() override;
	void OnEnabled() override;

	// return true if spawn
	bool SpawnRandom(Vector3 pos);

	static LootTable* GetInstance();
private:
	std::map<unsigned, GameObj*> _prefabs;

	static inline LootTable* __instance = nullptr;
	REGISTER_START(LootTable);
};

