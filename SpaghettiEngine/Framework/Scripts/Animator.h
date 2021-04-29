#pragma once
#include "Render2DScriptBase.h"
#include "Animation.h"

class Animator : public Render2DScriptBase
{
public:
	Animator();
	virtual void Update() override;
	virtual void Load(nlohmann::json& inputObject) override;
	virtual void Draw(SpriteHandler handler, PCamera camera) override;
protected:
	SAnimation _ani;
	SSprite _sprite;
	UINT frame = 0;
	double time = 0;
private:
	REGISTER_START(Animator);
};