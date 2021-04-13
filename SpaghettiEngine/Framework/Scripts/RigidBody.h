#pragma once
#include "ScriptBase.h"
#include "Physic.h"
#include "CornDirectX.h"

class Physic;

class Rigidbody : public ScriptBase
{
	friend class Physic;
public:
	Rigidbody();
	virtual void Update() override;
protected:

	Vector3 fall = Vector3(0.f, 1.f, 0.f);
	float movementSpeed = 0;
	float gravity = 10;
private:
	REGISTER_START(Rigidbody);
};