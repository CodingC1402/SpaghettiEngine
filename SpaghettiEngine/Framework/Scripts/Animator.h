#pragma once
#include "SpriteRenderer.h"
#include "Animation.h"

class Animator : public SpriteRenderer
{
public:
	Animator();
	virtual void Update() override;
	void Load(nlohmann::json& inputObject) override;
	virtual void Unload() override;
protected:
	SAnimation _ani;
	static SAnimation* test;
	UINT frame = 0;
	double time = 0;
private:
	REGISTER_START(Animator);
};