#pragma once
#include "ScriptBase.h"
#include "Camera.h"
#include "InputSystem.h"

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
	PCamera cam;

	Vector3 move;
	float movementSpeed = 70;
	bool isFlipped = false;
private:
	REGISTER_START(MoveScript);
};

