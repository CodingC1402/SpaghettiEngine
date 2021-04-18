#pragma once
#include "ScriptBase.h"
#include "Physic.h"

class Physic;

class BoxCollider : public ScriptBase
{
	friend class Physic;
public:
	BoxCollider();
	virtual void Start() override;
	virtual void Update() override;
protected:
	virtual void Load(const std::string* inputArg) override;
protected:
	float width;
	float height;
	float x;
	float y;
	float vx;
	float vy;
private:
	REGISTER_START(BoxCollider);
};