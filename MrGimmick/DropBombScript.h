#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "Polygon2DCollider.h"

class DropBombScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void OnCollide(CollideEvent& e) override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	WGameObj _currentStar;

	RigidBody2D* _rbBody = nullptr;
	Animator* _animator = nullptr;
	Polygon2DCollider* _polyCollider = nullptr;
	
	float _width;
	float _height;
	float _explodeTime;

	bool _isStarted = false;
	float _counter = 0;

	BoolField _explodedField;
	BoolField _dropField;
private:
	REGISTER_START(DropBombScript);
};