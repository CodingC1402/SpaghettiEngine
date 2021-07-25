#pragma once
#include "ScriptBase.h"

class MaxHealthPotion : public ScriptBase
{
public:
	void OnEnd() override;
private:
	REGISTER_START(MaxHealthPotion);
};

