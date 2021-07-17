#pragma once
#include "ScriptBase.h"
#include "CanonBallScript.h"
#include "RigidBody2D.h"

class ShootScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void OnDisabled() override;

	void Load(nlohmann::json& input);
protected:
	PGameObj		_starPrefab = nullptr;
	WGameObj		_currentBall;

	RigidBody2D* _rb = nullptr;
	CanonBallScript* _canonBallScript = nullptr;

	bool isFliped = false;

	Vector3			_appearOffSet;
private:
	REGISTER_START(ShootScript);
};