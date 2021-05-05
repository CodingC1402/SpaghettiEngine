#pragma once
#include "ScriptBase.h"
#include "CornException.h"
#include "Physic.h"

class Physic;

class RigidBody2D : public ScriptBase
{
	friend class Physic;
public:
	RigidBody2D(PScene owner);
	virtual void OnStart() override;
	virtual void OnUpdate() override;
	void SetVelocity(Vector3 v);
	void SetVelocityByDelta(Vector3 delta);
	Vector3 GetVelocity();
protected:
	virtual void Load(json& inputObject) override;
protected:
	Vector3 fall;
	float vx = 0;
	float vy = 0;
	float gravity = 10;
private:
	REGISTER_START(RigidBody2D);
};