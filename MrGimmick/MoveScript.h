#pragma once
#include "ScriptBase.h"
#include "Camera.h"
#include "InputSystem.h"
#include "RigidBody2D.h"

class MoveScript : public ScriptBase
{
public:
	MoveScript(PScene owner);
	void OnStart() override;
	void OnUpdate() override;
protected:
	SInput up;
	SInput down;
	SInput left;
	SInput right;
	PCamera cam;
	
	Vector3 move;
	float movementSpeed = 70;
	bool isFlipped = false;

	RigidBody2D* body;
private:
	REGISTER_START(MoveScript);
};

