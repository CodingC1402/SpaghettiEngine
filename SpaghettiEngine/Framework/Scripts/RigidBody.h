#pragma once
#include "ScriptBase.h"
#include "Physic.h"
#include "InputSystem.h"

class Physic;

class RigidBody : public ScriptBase
{
	friend class Physic;
public:
	RigidBody();
	virtual void Start() override;
	virtual void Update() override;
protected:
	SInput down;

	Vector3 fall;
	float movementSpeed = 0;
	float gravity = 10;
private:
	REGISTER_START(RigidBody);
};