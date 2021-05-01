#pragma once
#include "ScriptBase.h"

class RigidBody;

class BoxCollider : public ScriptBase
{
	friend class Physic;
public:
	BoxCollider();
	virtual void Start() override;
	virtual void Update() override;
	Vector3 GetPosition();
	Vector3 GetVelocity();
protected:
	virtual void Load(const std::string* inputArg) override;
protected:
	RigidBody* rigidbody;
	float width;
	float height;
	float vx;
	float vy;
private:
	REGISTER_START(BoxCollider);
};