#include "SegmentScript.h"
#include "FieldNames.h"
#include "Enemy.h"
#include "LoadingJson.h"

REGISTER_FINISH(SegmentScript, ScriptBase) {}

void SegmentScript::OnStart()
{
	__instances.insert(this);
	Spawn();
}

void SegmentScript::OnEnd()
{
	__instances.erase(this);
	DeSpawn();
}

void SegmentScript::OnEnabled()
{
	ScriptBase::OnEnabled();
	EnableEnemies();
}

void SegmentScript::OnDisabled()
{
	ScriptBase::OnDisabled();
	DisableEnemies();
}

void SegmentScript::Spawn()
{
	DeSpawn();
	for (auto& toSpawn : _enemySpawns)
	{
		_spawnedEnemies.push_back(
			std::dynamic_pointer_cast<GameObj>(GetOwner()->Instantiate(_prefabs[toSpawn.first], toSpawn.second)->GetSharedPtr())
		);
	}
}

void SegmentScript::DeSpawn()
{
	for (auto& enemy : _spawnedEnemies)
	{
		if (!enemy.expired())
			enemy.lock()->CallDestroy();
	}
}

void SegmentScript::EnableEnemies()
{
	LoopEnemies([](WGameObj obj) {
		obj.lock()->Enable();
	});
}

void SegmentScript::DisableEnemies()
{
	LoopEnemies([](WGameObj obj) {
		auto enemyScript = dynamic_cast<Enemy*>(obj.lock()->GetScriptContainer().GetItemType(TYPE_NAME(Enemy)));
		if (enemyScript->IsInTube())
			obj.lock()->CallDestroy();
		else
			obj.lock()->Disable();
	});
}

void SegmentScript::LoopEnemies(std::function<void(WGameObj)> function)
{
	WGameObj obj;
	for (auto it = _spawnedEnemies.begin(); it != _spawnedEnemies.end();)
	{
		if ((*it).expired())
			it = _spawnedEnemies.erase(it);
		else
		{
			obj = (*it).lock();
			function(obj);
			++it;
		}
	}
}

void SegmentScript::Load(nlohmann::json& input)
{
	for (auto& prefab : input[LoadingJson::Field::gameObjectsField])
	{
		_prefabs.push_back(
			dynamic_cast<GameObj*>(GetOwner()->GetComponent(prefab[LoadingJson::Field::idField].get<CULL>()))
		);
	}

	for (auto& spawn : input[Fields::SegmentScript::_enenySpawns])
	{
		_enemySpawns.emplace_back(
			spawn[Fields::SegmentScript::_index].get<unsigned>(),
			spawn[Fields::SegmentScript::_position]
		);
	}
}

ScriptBase* SegmentScript::Clone() const
{
	auto clone = dynamic_cast<SegmentScript*>(ScriptBase::Clone());

	clone->_prefabs = _prefabs;
	clone->_enemySpawns = _enemySpawns;

	return clone;
}

void SegmentScript::SpawnAll()
{
	for (auto& instance : __instances)
		instance->Spawn();
}

void SegmentScript::DespawnAll()
{
	for (auto& instance : __instances)
		instance->DeSpawn();
}