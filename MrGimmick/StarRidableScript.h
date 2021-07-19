#pragma once
#include "ScriptBase.h"

class StarRidableScript : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
private:
	REGISTER_START(StarRidableScript);
};

