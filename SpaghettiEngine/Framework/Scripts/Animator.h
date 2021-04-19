#pragma once
#include "SpriteRenderer.h"
#include "Animation.h"

class Animator : SpriteRenderer
{
public:
	virtual void Update() override;
protected:
	
private:
	REGISTER_START(Animator);
};