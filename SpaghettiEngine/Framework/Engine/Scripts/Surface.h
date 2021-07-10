#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include <set>

class Surface : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
	void OnFixedUpdate() override;
protected:
	std::set<RigidBody2D*> _collided;
	std::set<RigidBody2D*> _collideNow;
	float _increaseVel = 20;
private:
	REGISTER_START(Surface);
};