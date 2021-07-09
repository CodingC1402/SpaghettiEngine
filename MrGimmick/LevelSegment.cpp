#include "LevelSegment.h"

REGISTER_FINISH(LevelSegment);

void LevelSegment::PlayerEnter()
{
	for (auto& collider : _colliders)
		collider->Enable();
	for (auto& enemy : _enemyPrefabs)
		_currentEnemies.emplace_back(GetOwner()->Instantiate(enemy.first, enemy.second));
}

void LevelSegment::PlayerExit()
{
	for (auto& collider : _colliders)
		collider->Disable();
	for (auto& enemy : _currentEnemies)
		enemy->CallDestroy();
	_currentEnemies.clear();
}

LevelSegment::LevelSegment(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}
