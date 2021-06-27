#pragma once
#include "Render2DScriptBase.h"
#include "StarScript.h"
#include "Animation.h"

class StarCreation : public Render2DScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	StarScript* _starScript = nullptr;
	SAnimation _starAnim;

	float _createTime = 0;
	float _counter = 0;
private:
	REGISTER_START(StarCreation);
};

