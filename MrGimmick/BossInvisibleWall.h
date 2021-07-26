#pragma once
#include "ScriptBase.h"

class BossInvisibleWall : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;;
private:
	REGISTER_START(BossInvisibleWall);
};

