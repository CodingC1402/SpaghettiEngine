#include "SpawnPoint.h"
#include "FieldNames.h"
#include "PhysicCollide.h"
#include <set>

REGISTER_FINISH(SpawnPoint, ScriptBase) {}

void SpawnPoint::Load(nlohmann::json& input)
{
	_center = input[Fields::SpawnPoint::_center];
	_width = input[Fields::SpawnPoint::_width].get<unsigned>();
	_height = input[Fields::SpawnPoint::_height].get<unsigned>();

	_spawnPos = input[Fields::SpawnPoint::_spawnPoint];
}

void SpawnPoint::OnFixedUpdate()
{
	if (_spawnPointSetted)
		return;

	std::set<GameObj*> objList;
	PhysicCollide::GetCollidedWithRectangle(
		GetGameObject(),
		objList,
		_center,
		_width,
		_height,
		Fields::SpecialTag::GetPlayerTag(),
		PhysicCollide::FilterMode::Collide
	);

	if (!objList.empty())
		SetSpawnPoint(this);
}

void SpawnPoint::SetSpawnPoint(SpawnPoint* spawnPointScript)
{
	if (__currentSpawn)
		__currentSpawn->GetGameObject()->CallDestroy();
	__currentSpawn = spawnPointScript;
	spawnPointScript->_spawnPointSetted = true;
}

Vector3 SpawnPoint::GetSpawnPosition()
{
	return _spawnPos;
}

SpawnPoint* SpawnPoint::GetSpawnPointScript()
{
	return __currentSpawn;
}
