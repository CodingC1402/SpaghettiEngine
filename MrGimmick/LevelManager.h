#pragma once
#include "ScriptBase.h"

class LevelManager : public ScriptBase
{
public:
	void OnStart() override;
	void OnEnabled() override;
	void Spawn();
	void DeSpawn();
	void Load(nlohmann::json& input) override;

	ScriptBase* Clone() const override;
	[[nodiscard]] static LevelManager* GetCurrentScript();
private:
	std::vector<GameObj*> _prefabs;
	// enemy to spawn and position to spawn.
	std::vector<std::pair<unsigned, Vector3>> _enemySpawns;
	std::vector<WGameObj> _spawnedEnemies;

	static inline LevelManager* __currentLevelManager;
	REGISTER_START(LevelManager);
};

