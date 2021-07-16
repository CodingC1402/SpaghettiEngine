#include "LevelSegment.h"
#include "LoadingJson.h"
#include "Polygon2DCollider.h"

REGISTER_FINISH(LevelSegment, ScriptBase) {}

void LevelSegment::OnEnabled()
{
	for (auto& enemy : _spawnPoint)
		_currentEnemies.push_back(GetOwner()->Instantiate(_enemyPrefabs[enemy.first], enemy.second));
}

void LevelSegment::OnDisabled()
{
	for (auto& enemy : _currentEnemies)
		enemy->CallDestroy();
}

void LevelSegment::Load(nlohmann::json& input)
{
	for (auto& obj : input[LoadingJson::Field::gameObjectsField])
		_enemyPrefabs.push_back(dynamic_cast<GameObj*>(GetOwner()->GetComponent(obj[LoadingJson::Field::idField].get<CULL>())));


}
