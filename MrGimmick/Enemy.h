#pragma once
#include "ScriptBase.h"

class Enemy : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
	void OnUpdate() override;
private:
	REGISTER_START(Enemy);
};

