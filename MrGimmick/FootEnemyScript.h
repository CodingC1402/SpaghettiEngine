#pragma once
#include "ScriptBase.h"
#include "HealthScript.h"
#include "MoveScript.h"
#include "RigidBody2D.h"
#include "Animator.h"

// Used for enemy that can walk, not include canon balls.
class FootEnemyScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;

	void Died();
private:
	float _destroyDelay = 0.0f;
	float _destroyCounter = 0.0f;
	Vector3 _hurtVel;

	HealthScript* _healthScript = nullptr;
	MoveScript* _moveScript = nullptr;
	RigidBody2D* _rb = nullptr;
	Animator* _animator = nullptr;

	std::string _diedFieldName = "";
	BoolField _isDead;
	BoolField _isFlipped;

	REGISTER_START(FootEnemyScript);
};

