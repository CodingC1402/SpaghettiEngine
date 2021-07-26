#pragma once
#include "ScriptBase.h"
#include "AIScript.h"
#include "MoveScript.h"
#include "RigidBody2D.h"
#include "HealthScript.h"
#include <memory>
#include <list>

class Boss1Script : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void OnDisabled() override;

	ScriptBase* Clone() const override;
	void Load(nlohmann::json& input) override;
private:
	Vector3 _force;
	Matrix4 _flipForceMatrix;
	Matrix4 _flipXMatrix;
	Matrix4 _rotateMatrix;

	GameObj* _normalEnemyPrefab = nullptr;
	GameObj* _electricPrefab = nullptr;
	GameObj* _loadNextScenePrefab = nullptr;

	HealthScript* _healthScript = nullptr;
	MoveScript* _moveScript = nullptr;
	RigidBody2D* _rb = nullptr;

	BoolField _readyAttack;

	float _delay = 0.5;
	float _delayCounter = 0;
	float _range = 100;

	float _angle = 120;
	unsigned _count = 3;

	float _currentAngle = 0;
	Vector3 _currentForceVec;
	unsigned _shotCounter = 0;
	std::list<WGameObj> _created;

private:
	REGISTER_START(Boss1Script);
};

