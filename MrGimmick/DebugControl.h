#pragma once
#include "ScriptBase.h"
#include "InputAll.h"

#include "PlayerControl.h"
#include "RigidBody2D.h"

class DebugControl : public ScriptBase
{
public:
	void OnStart();
	void OnUpdate();
	void OnFixedUpdate();
	void Load(nlohmann::json& input) override;
private:
	SInputAll _moveUp;
	SInputAll _moveDown;
	SInputAll _moveRight;
	SInputAll _moveLeft;
	SInputAll _enterDebug;

	float _debugFlySpeed = 0;
	bool _isInDebugMode = false;

	PlayerControl* _playerControlScript = nullptr;
	RigidBody2D* _rbScript = nullptr;

	REGISTER_START(DebugControl);
};

