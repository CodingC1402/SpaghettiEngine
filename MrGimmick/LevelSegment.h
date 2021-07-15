#pragma once
#include "ScriptBase.h"

class LevelSegment : public ScriptBase
{
public:
	void OnStart();
	void PlayerEnter();
	void PlayerExit();
protected:
	std::list<ScriptBase*> _colliders;
	std::vector<std::pair<GameObj*, Vector3>> _enemyPrefabs;
	std::vector<GameObj*> _currentEnemies;
private:
	REGISTER_START(LevelSegment);
};

