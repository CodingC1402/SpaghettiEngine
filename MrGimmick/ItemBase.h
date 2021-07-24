#pragma once
#include "ScriptBase.h"

class ItemBase : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
private:
	REGISTER_START(ItemBase);
};

