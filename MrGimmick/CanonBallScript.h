#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "CircleCollider.h"
#include "Polygon2DCollider.h"

class CanonBallScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;

	void Load(nlohmann::json& input) override;
	void Throw(const Vector3& _playerVel, bool isFlipped);
	void OnCollide(CollideEvent& e) override;
	void DisableBeforeExplode();
	PScriptBase Clone() const override;
protected:
	RigidBody2D* _rbBody = nullptr;
	Animator* _animator = nullptr;
	CircleCollider* _cirCollider = nullptr;
	Polygon2DCollider* _polyCollider = nullptr;
	BoolField _explodedField;

	bool _destroyed = false;
	bool _counterStarted = false;
	float _counter = 0;
	float _explodeTime = 10; // In second
	float _animExplodeTime = 1;

	float _additionVel = 30; // Add speed each update  to replenish the x axis vel

	Vector3	_startVelocity;
private:
	REGISTER_START(CanonBallScript);
};