#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class EnemyJumpScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	ScriptBase* Clone() const override;

	void Load(nlohmann::json& input) override;
private:
	Vector3 _groundScanRect;
	float	_groundScanRectWidth = 0;
	float	_groundScanRectHeight = 0;

	Vector3 _wallScanRect;
	float	_wallScanRectWidth = 0;
	float	_wallScanRectHeight = 0;

	float	_jumpAgainDelay = 0;
	float	_jumpAgainAccumulative = 0;

	bool _lastJump = false;
	MoveScript* _moveScript = nullptr;

	REGISTER_START(EnemyJumpScript);
};

