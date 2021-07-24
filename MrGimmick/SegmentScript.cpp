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
	for (auto it = _enemySpawns.begin(); it != _enemySpawns.end();)
	{
		auto& toSpawn = *it;
		_spawnedEnemies.push_back(
			std::dynamic_pointer_cast<GameObj>(GetOwner()->Instantiate(_prefabs[toSpawn.first].first, toSpawn.second)->GetSharedPtr())
		);
		if (_prefabs[toSpawn.first].second)
			it = _enemySpawns.erase(it);
		else
			++it;
	}
	if (IsDisabled())
		DisableEnemies();
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
	bool isSingleUse;
	for (auto& prefab : input[LoadingJson::Field::gameObjectsField])
	{
		isSingleUse = prefab[Fields::SegmentScript::_singleUse].empty() ? false : prefab[Fields::SegmentScript::_singleUse].get<bool>();
		_prefabs.emplace_back(
			dynamic_cast<GameObj*>(GetOwner()->GetComponent(prefab[LoadingJson::Field::idField].get<CULL>())),
			isSingleUse
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

void SegmentScript::DisableAllSegment()
{
	for (auto& instance : __instances)
		instance->GetGameObject()->Disable();
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