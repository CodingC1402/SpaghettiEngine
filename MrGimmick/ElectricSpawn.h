#pragma once
#include "ScriptBase.h"
#include "CRectangle.h"

class ElectricSpawn : public ScriptBase
{
public:
	struct DoorWay
	{
		bool _opened = false;
		GameObj* _door = nullptr;
		CRectangle _rect;
		Vector3 _spawnPoint;
	};
public:
	void OnStart() override;

	void OnEnabled() override;
	void OnDisabled() override;

	ScriptBase* Clone() const override;

	void OnFixedUpdate() override;
	void Load(nlohmann::json& input) override;
private:
	GameObj* _electricPrefab = nullptr;
	std::list<WGameObj> _spawnedEnemies;
	DoorWay _doorWay1;
	DoorWay _doorWay2;

	REGISTER_START(ElectricSpawn);
};

