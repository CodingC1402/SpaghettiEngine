#include "LevelManager.h"
#include "FieldNames.h"
#include "LoadingJson.h"

REGISTER_FINISH(LevelManager, ScriptBase) {}

void LevelManager::OnStart()
{
	__currentLevelManager = this;
	Spawn();
}

void LevelManager::OnEnabled()
{

}

void LevelManager::Spawn()
{
	DeSpawn();
	for (auto& toSpawn : _enemySpawns)
	{
		_spawnedEnemies.push_back(
			std::dynamic_pointer_cast<GameObj>(GetOwner()->Instantiate(_prefabs[toSpawn.first], toSpawn.second)->GetSharedPtr())
		);
	}
}

void LevelManager::DeSpawn()
{
	for (auto& enemy : _spawnedEnemies)
	{
		if (!enemy.expired())
			enemy.lock()->CallDestroy();
	}
}

void LevelManager::Load(nlohmann::json& input)
{
	for (auto& prefab : input[LoadingJson::Field::gameObjectsField])
	{
		_prefabs.push_back(
			dynamic_cast<GameObj*>(GetOwner()->GetComponent(prefab[LoadingJson::Field::idField].get<CULL>()))
		);
	}

	for (auto& spawn : input[Fields::LevelManager::_enenySpawns])
	{
		_enemySpawns.emplace_back(
			spawn[Fields::LevelManager::_index].get<unsigned>(),
			spawn[Fields::LevelManager::_position]
		);
	}
}

ScriptBase* LevelManager::Clone() const
{
	auto clone = dynamic_cast<LevelManager*>(ScriptBase::Clone());

	clone->_prefabs = _prefabs;
	clone->_enemySpawns = _enemySpawns;

	return clone;
}

LevelManager* LevelManager::GetCurrentScript()
{
	return __currentLevelManager;
}
