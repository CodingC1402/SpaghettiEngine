#pragma once
#include "ScriptBase.h"

class LevelSegment : public ScriptBase
{
public:
	void OnEnabled() override;
	void OnDisabled() override;
	void Load(nlohmann::json& input) override;
protected:
	std::vector<GameObj*> _enemyPrefabs;
	std::vector<std::pair<unsigned, Vector3>> _spawnPoint;
	std::vector<GameObj*> _currentEnemies;
private:
	REGISTER_START(LevelSegment);
};

