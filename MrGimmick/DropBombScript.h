#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "Polygon2DCollider.h"

class DropBombScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	RigidBody2D* _rbBody = nullptr;
	Animator* _animator = nullptr;
	Polygon2DCollider* _polyCollider = nullptr;
	
	float _counter;

	BoolField _explodedField;
private:
	REGISTER_START(DropBombScript);
};