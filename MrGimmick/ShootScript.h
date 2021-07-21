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
	void OnFixedUpdate() override;
	void OnDisabled() override;

	void OnCollide(CollideEvent& e) override;
	ScriptBase* Clone() const override;

	void Load(nlohmann::json& input);
protected:
	PGameObj _starPrefab = nullptr;

	BoolField _isReloadedField;

	Animator* _animator = nullptr;
	RigidBody2D* _rb = nullptr;

	bool isFliped = false;

	float _currentTime = 0;
	float _reloadTime = 2.0f;

	std::list<WGameObj> _firedCanonBalls;
	Vector3			_appearOffSet;
private:
	REGISTER_START(ShootScript);
};