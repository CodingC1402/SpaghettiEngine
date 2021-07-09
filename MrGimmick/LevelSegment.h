#pragma once
#include "ScriptBase.h"
#include "Polygon2DCollider.h"

class LevelSegment : public ScriptBase
{
public:
	LevelSegment(PScene owner, bool isDisabled = false);
	void PlayerEnter();
	void PlayerExit();
protected:
	std::vector<Polygon2DCollider*> _colliders;
	std::vector<std::pair<GameObj*, Vector3>> _enemyPrefabs;
	std::vector<GameObj*> _currentEnemies;
private:
	REGISTER_START(LevelSegment);
};

