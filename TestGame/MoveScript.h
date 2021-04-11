#pragma once
#include "ScriptBase.h"
#include "InputSystem.h"

class MoveScript : public ScriptBase
{
public:
	virtual void Start() override;
	virtual void Update() override;
protected:
	SInput up;
	SInput down;
	SInput left;
	SInput right;

	Vector3 move;
	float movementSpeed = 2;
private:
	REGISTER_START(MoveScript);
};

