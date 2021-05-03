#pragma once
#include "Collider2DScriptBase.h"

class RigidBody;

class BoxCollider : public Collider2DScriptBase
{
	friend class Physic;
public:
	BoxCollider();
	void Start() override;
	Vector3* GetPosition() const;
	Vector3 GetVelocity();
	void DrawBox(RenderDevice render, PCamera camera, Color color) override;
protected:
	void Load(const std::string* inputArg) override;
protected:
	RigidBody* rigidbody;
	float width;
	float height;
	float vx;
	float vy;
private:
	REGISTER_START(BoxCollider);
};