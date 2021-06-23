#pragma once
#include "ScriptBase.h"

class StarScript : public ScriptBase
{
public:
	StarScript(PScene owner, bool isDisabled = false);

	void OnEnabled() override;
	void OnUpdate() override;

	void OnCollide(CollideEvent& e) override;
	PScriptBase Clone() const override;
protected:
	Vector3 originalPos;
	bool onWayBack = false;
private:
	REGISTER_START(StarScript);
};

