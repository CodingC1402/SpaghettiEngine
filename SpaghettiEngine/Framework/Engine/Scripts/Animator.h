#pragma once
#include "Render2DScriptBase.h"
#include "Animation.h"

class Animator : public Render2DScriptBase
{
public:
	Animator(PScene owner);
	virtual void OnUpdate() override;
	virtual void Load(nlohmann::json& inputObject) override;
	virtual void Draw(PCamera camera) override;

	PScriptBase Clone() const override;
protected:
	SAnimation _ani;
	SSprite _sprite;
	UINT frame = 0;
	float time = 0;
private:
	REGISTER_START(Animator);
};