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
	virtual void OnEnabled() override;
	virtual void OnDisabled() override;

	void SetConstVelocity(Vector3 v);
	void SetVelocity(Vector3 v);
	void AddForce(Vector3 delta);
	void SetMass(float m);

	Vector3 GetVelocity();
	Vector3 GetConstVelocity();
	float GetMass();

	virtual void Load(json& inputObject) override;
protected:
	Vector3 constVelocity;
	Vector3 velocity;
	float gravity = 10;
	float mass = 10;
private:
	REGISTER_START(RigidBody2D);
};