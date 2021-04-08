#pragma once
#include "ScriptBase.h"
#include "Graphics.h"
#include "Sprite.h"

class SpriteRenderer : ScriptBase
{
public:
	SpriteRenderer();
	virtual void Load(const std::string* inputArg, int argS) override;
	virtual void Update() override;
protected:
	Matrix tranformMatrix;
	SSprite sprite;
private:
	REGISTER_START(SpriteRenderer);
};
