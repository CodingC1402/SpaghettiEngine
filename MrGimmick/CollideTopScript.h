#pragma once
#include "ScriptBase.h"

class CollideTopScript : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
private:
	REGISTER_START(CollideTopScript);
};

