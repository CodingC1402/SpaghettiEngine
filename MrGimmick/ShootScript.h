#pragma once
#include "ScriptBase.h"
#include "CanonBallScript.h"
#include "RigidBody2D.h"
#include "Animator.h"

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

	BoolField _isReloadedField;

	Animator* _animator = nullptr;
	RigidBody2D* _rb = nullptr;
	std::list<CanonBallScript*> _canonBallScriptList;
	//CanonBallScript* _canonBallScript = nullptr;

	bool isFliped = false;

	float _currentTime = 0;
	float _reloadTime = 2.0f;

	Vector3			_appearOffSet;
private:
	REGISTER_START(ShootScript);
};