#pragma once
#include "ScriptBase.h"
#include "Debug.h"
#include "MoveScript.h"
#include <set>

class RidableScript : public ScriptBase
{
public:
	void OnEnabled() override;
	void OnFixedUpdate() override;
	void OnCollide(CollideEvent& e) override;
private:
	Vector3 _delta;
	Vector3 _oldPos;
	std::set<GameObj*> _objects;

	REGISTER_START(RidableScript);
};