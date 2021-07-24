#include "SpawnPoint.h"
#include "FieldNames.h"
#include "PhysicCollide.h"
#include "Setting.h"
#include "DebugRenderer.h"
#include "LoadingJson.h"
#include <set>

REGISTER_FINISH(SpawnPoint, ScriptBase) {}
constexpr unsigned DEBUG_COLOR = 0xFF00FF00;
constexpr unsigned DEBUG_RADIUS = 10;

void SpawnPoint::Load(nlohmann::json& input)
{
	_center = input[Fields::SpawnPoint::_center];
	_width = input[Fields::SpawnPoint::_width].get<float>();
	_height = input[Fields::SpawnPoint::_height].get<float>();

	_spawnPos = input[Fields::SpawnPoint::_spawnPoint];
	_segment = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
}

void SpawnPoint::OnFixedUpdate()
{
	if (_spawnPointSetted)
	{
		if constexpr (Setting::IsDebugMode())
		{
			Matrix4 matrix = Matrix4::GetDiagonalMatrix();
			matrix._41 = _spawnPos.x;
			matrix._42 = _spawnPos.y;
			matrix._43 = _spawnPos.z;

			DebugRenderer::DrawCircle(DEBUG_RADIUS, matrix, DEBUG_COLOR);
		}
		return;
	}

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

	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangleFromCenter(_center, _width, _height, GetWorldMatrix(), DEBUG_COLOR);
	}

	if (!objList.empty())
		SetSpawnPoint(this);
}

void SpawnPoint::OnDisabled()
{
	__currentSpawn = __currentSpawn == this ? nullptr : __currentSpawn;
}

void SpawnPoint::SetSpawnPoint(SpawnPoint* spawnPointScript)
{
	if (__currentSpawn)
		__currentSpawn->CallDestroy();
	__currentSpawn = spawnPointScript;
	spawnPointScript->_spawnPointSetted = true;
}

Vector3 SpawnPoint::GetSpawnPosition()
{
	return _spawnPos;
}

GameObj* SpawnPoint::GetSegment()
{
	return _segment;
}

SpawnPoint* SpawnPoint::GetSpawnPointScript()
{
	return __currentSpawn;
}
