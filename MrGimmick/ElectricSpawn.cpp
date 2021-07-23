#include "ElectricSpawn.h"
#include "PhysicCollide.h"
#include "DebugRenderer.h"
#include "Setting.h"
#include "LoadingJson.h"
#include "FieldNames.h"

REGISTER_FINISH(ElectricSpawn, ScriptBase) {}
static constexpr unsigned DEBUG_COLOR = 0xFFFFFF00;
static constexpr float DEBUG_RADIUS = 8.0f;

void ElectricSpawn::OnStart()
{
	_doorWay1._door->Disable();
	_doorWay2._door->Disable();
}

void ElectricSpawn::OnEnabled()
{
	for (auto& spawned : _spawnedEnemies)
	{
		if (!spawned.expired())
			spawned.lock()->Enable();
	}
}

void ElectricSpawn::OnDisabled()
{
	for (auto& spawned : _spawnedEnemies)
	{
		if (!spawned.expired())
			spawned.lock()->Disable();
	}
}

ScriptBase* ElectricSpawn::Clone() const
{
	auto clone = dynamic_cast<ElectricSpawn*>(ScriptBase::Clone());

	clone->_doorWay1 = _doorWay1;
	clone->_doorWay2 = _doorWay2;
	clone->_electricPrefab = _electricPrefab;

	clone->_doorWay1._opened = false;
	clone->_doorWay2._opened = false;

	return clone;
}

void ElectricSpawn::OnFixedUpdate()
{
	if constexpr (Setting::IsDebugMode())
	{
		if (!_doorWay1._opened)
		{
			DebugRenderer::DrawRectangleFromCenter(
				_doorWay1._rect.GetCenter(),
				_doorWay1._rect.GetWidth(),
				_doorWay1._rect.GetHeight(),
				_doorWay1._door->GetTransform().GetWorldMatrix(),
				DEBUG_COLOR
			);

			DebugRenderer::DrawCircle(
				DEBUG_RADIUS,
				_doorWay1._spawnPoint,
				Matrix4::GetDiagonalMatrix(),
				DEBUG_COLOR
			);
		}
		if (!_doorWay2._opened)
		{
			DebugRenderer::DrawRectangleFromCenter(
				_doorWay2._rect.GetCenter(),
				_doorWay2._rect.GetWidth(),
				_doorWay2._rect.GetHeight(),
				_doorWay2._door->GetTransform().GetWorldMatrix(),
				DEBUG_COLOR
			);

			DebugRenderer::DrawCircle(
				DEBUG_RADIUS,
				_doorWay2._spawnPoint,
				Matrix4::GetDiagonalMatrix(),
				DEBUG_COLOR
			);
		}
	}

	std::set<GameObj*> listOfObj;
	if (!_doorWay1._opened)
	{
		PhysicCollide::GetCollidedWithRectangle(
			_doorWay1._door,
			listOfObj,
			_doorWay1._rect.GetCenter(),
			_doorWay1._rect.GetWidth(),
			_doorWay1._rect.GetHeight(),
			Fields::SpecialTag::GetPlayerTag(),
			PhysicCollide::FilterMode::Collide
		);

		if (!listOfObj.empty())
		{
			_doorWay1._opened = true;
			_doorWay1._door->Enable();
			_spawnedEnemies.push_back(std::dynamic_pointer_cast<GameObj>(GetOwner()->Instantiate(_electricPrefab, _doorWay1._spawnPoint)->GetSharedPtr()));
		}
	}
	listOfObj.clear();
	if (!_doorWay2._opened)
	{
		PhysicCollide::GetCollidedWithRectangle(
			_doorWay2._door,
			listOfObj,
			_doorWay2._rect.GetCenter(),
			_doorWay2._rect.GetWidth(),
			_doorWay2._rect.GetHeight(),
			Fields::SpecialTag::GetPlayerTag(),
			PhysicCollide::FilterMode::Collide
		);

		if (!listOfObj.empty())
		{
			_doorWay2._opened = true;
			_doorWay2._door->Enable();
			_spawnedEnemies.push_back(std::dynamic_pointer_cast<GameObj>(GetOwner()->Instantiate(_electricPrefab, _doorWay2._spawnPoint)->GetSharedPtr()));
		}
	}
}

void ElectricSpawn::Load(nlohmann::json& input)
{
	auto gameObjs = input[LoadingJson::Field::gameObjectsField];
	_electricPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(gameObjs[0][LoadingJson::Field::idField].get<CULL>()));
	_doorWay1._door = dynamic_cast<GameObj*>(GetOwner()->GetComponent(gameObjs[1][LoadingJson::Field::idField].get<CULL>()));
	_doorWay2._door = dynamic_cast<GameObj*>(GetOwner()->GetComponent(gameObjs[2][LoadingJson::Field::idField].get<CULL>()));

	_doorWay1._rect = input[Fields::ElectricSpawn::_doorWay1];
	_doorWay1._spawnPoint = input[Fields::ElectricSpawn::_doorWay1][Fields::ElectricSpawn::_spawn];

	_doorWay2._rect = input[Fields::ElectricSpawn::_doorWay2];
	_doorWay2._spawnPoint = input[Fields::ElectricSpawn::_doorWay2][Fields::ElectricSpawn::_spawn];
}
