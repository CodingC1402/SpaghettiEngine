#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "Polygon2DCollider.h"
#include "CRectangle.h"

class AttackScript : public ScriptBase
{
	friend class StarCreation;
public:
	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;
	void Explode();

	void Load(nlohmann::json& input) override;
	void Throw(const Vector3& _playerVel, bool isFlipped);
	void OnCollide(CollideEvent& e) override;
	PScriptBase Clone() const override;
protected:
	void SetCreated();
protected:
	RigidBody2D* _rbBody			= nullptr;
	Animator* _animator				= nullptr;
	Polygon2DCollider* _polyCollider= nullptr;
	BoolField _explodedField;
	CRectangle _cancelZone;

	bool _exploded			= false;
	bool _counterStarted	= false;
	float _counter			= 0;
	float _explodeTime		= 10; // In second
	float _animExplodeTime	= 1;

	float _maxDistance		= 0;
	float _additionVel		= 30; // Add speed each update  to replenish the x axis vel

	float _usableCounter	= 0;
	float _beforeUsable		= 0;
	bool  _countUsable		= false;

	Vector3	_startVelocity;
private:
	REGISTER_START(AttackScript);
};

