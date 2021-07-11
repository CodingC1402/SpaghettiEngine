#pragma once
#include "ScriptBase.h"
#include "RigidBody2D.h"
#include <set>

class Surface : public ScriptBase
{
public:
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};
public:
	void OnCollide(CollideEvent& e) override;
	void OnFixedUpdate() override;
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;
private:
	std::set<RigidBody2D*> _collided;
	std::set<RigidBody2D*> _collideNow;

	Tag _ignoreTags;
	Vector3 _moveVec;
	Direction _direction;
private:
	REGISTER_START(Surface);
};