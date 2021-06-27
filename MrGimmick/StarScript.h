#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "Polygon2DCollider.h"

class StarScript : public ScriptBase
{
	friend class StarCreation;
public:
	StarScript(PScene owner, bool isDisabled = false);

	void OnStart() override;
	void OnUpdate() override;

	void Load(nlohmann::json& input) override;
	void Throw(const Vector3& _playerVel);
	void OnCollide(CollideEvent& e) override;
	PScriptBase Clone() const override;
protected:
	void SetCreated();
protected:
	RigidBody2D*		_rbBody = nullptr;
	Animator*			_animator = nullptr;
	Polygon2DCollider*	_polyCollider = nullptr;
	BoolField			_explodedField;

	bool				_counterStarted = false;
	float				_beforeUsable = false;
	float				_counter = 0;
	float				_explodeTime = 10; // In second
	float				_animExplodeTime = 1;

	Vector3				_startVelocity;
private:
	REGISTER_START(StarScript);
};

