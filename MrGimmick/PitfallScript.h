#pragma once
#include "ScriptBase.h"
#include <set>

class PitfallScript : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
	void OnFixedUpdate() override;
private:
	std::set<GameObj*> _collided;

	REGISTER_START(PitfallScript);
};

