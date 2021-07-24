#pragma once
#include "ScriptBase.h"

class SpawnPoint : public ScriptBase
{
public:
	void Load(nlohmann::json& input) override;
	void OnFixedUpdate() override;
	void OnDisabled() override;

	static void SetSpawnPoint(SpawnPoint* spawnPointScript);
	[[nodiscard]] Vector3 GetSpawnPosition();
	[[nodiscard]] GameObj* GetSegment();

	[[nodiscard]] static SpawnPoint* GetSpawnPointScript();
private:
	// Trigger area
	Vector3 _center;
	float _width = 0;
	float _height = 0;

	// Spawn point
	Vector3 _spawnPos;
	bool _spawnPointSetted = false;

	// Segment that this spawn point is in
	GameObj* _segment = nullptr;

	static inline SpawnPoint* __currentSpawn = nullptr;
	REGISTER_START(SpawnPoint);
};

