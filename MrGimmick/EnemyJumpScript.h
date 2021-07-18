#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class EnemyJumpScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;
private:
	Vector3 _groundScanRect;
	float	_groundScanRectWidth;
	float	_groundScanRectHeight;

	Vector3 _wallScanRect;
	float	_wallScanRectWidth;
	float	_wallScanRectHeight;

	bool _lastJump = false;
	MoveScript* _moveScript = nullptr;

	REGISTER_START(EnemyJumpScript);
};

