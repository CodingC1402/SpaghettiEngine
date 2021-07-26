#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "Polygon2DCollider.h"
#include "CRectangle.h"

class DropBombScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void OnCollide(CollideEvent& e) override;
	void Explode();
	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	WGameObj _currentStar;

	RigidBody2D* _rbBody = nullptr;
	Animator* _animator = nullptr;
	Polygon2DCollider* _polyCollider = nullptr;
	Polygon2DCollider* _afterExplodeCollider = nullptr;
	
	CRectangle _rect;
	float _explodeTime = 0.0f;

	bool _exploded = false;
	bool _isStarted = false;
	float _counter = 0;

	float _levelRadius = 0.0f;
	unsigned _totalFanLevel = 0;

	UnsignedField _fanSpeed;
	BoolField _explodedField;
	BoolField _dropField;
private:
	REGISTER_START(DropBombScript);
};