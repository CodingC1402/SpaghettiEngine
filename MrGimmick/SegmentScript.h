#pragma once
#include "ScriptBase.h"
#include <functional>

class SegmentScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnEnd() override;
	void OnEnabled() override;
	void OnDisabled() override;

	void Spawn();
	void DeSpawn();
	void EnableEnemies();
	void DisableEnemies();
	void LoopEnemies(std::function<void(WGameObj)> function);

	void Load(nlohmann::json& input) override;

	ScriptBase* Clone() const override;

	static void DisableAllSegment();
	static void SpawnAll();
	static void DespawnAll();
protected:
	// the bool indicate whether the prefabs is single use; true = single use
	std::vector<std::pair<GameObj*, bool>> _prefabs;
	// enemy to spawn and position to spawn.
	std::vector<std::pair<unsigned, Vector3>> _enemySpawns;
	std::vector<WGameObj> _spawnedEnemies;
	static inline std::set<SegmentScript*> __instances;

private:
	REGISTER_START(SegmentScript);
};
