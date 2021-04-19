#pragma once
#include "ScriptBase.h"
#include "InputSystem.h"
#include "Physic.h"

class MoveScript : public ScriptBase
{
public:
	MoveScript();
	virtual void Start() override;
	virtual void Update() override;
protected:
	SInput up;
	SInput down;
	SInput left;
	SInput right;

	Vector3 move;
	float movementSpeed = 50;
private:
	REGISTER_START(MoveScript);
};

