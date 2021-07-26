#pragma once
#include "ScriptBase.h"

class LoadNextScene : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
private:
	REGISTER_START(LoadNextScene);
};

