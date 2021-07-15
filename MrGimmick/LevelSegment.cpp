#include "LevelSegment.h"
#include "Polygon2DCollider.h"

REGISTER_FINISH(LevelSegment, ScriptBase) {}

void LevelSegment::OnStart()
{
	_colliders = GET_ALL_SCRIPTS_OF_TYPE(Polygon2DCollider);
}

void LevelSegment::PlayerEnter()
{
	for (auto& collider : _colliders)
		collider->Enable();
	//for (auto& enemy : _enemyPrefabs)
	//	_currentEnemies.emplace_back(GetOwner()->Instantiate(enemy.first, enemy.second));
}

void LevelSegment::PlayerExit()
{
	for (auto& collider : _colliders)
		collider->Disable();
	//for (auto& enemy : _currentEnemies)
	//	enemy->CallDestroy();
	//_currentEnemies.clear();
}
