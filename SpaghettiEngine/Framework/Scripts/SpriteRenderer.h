#pragma once
#include "ScriptBase.h"
#include "Graphics.h"
#include "Sprite.h"
#include "CornDirectX.h"

class Graphics;

class SpriteRenderer : public ScriptBase
{
	friend class Graphics;
public:
	SpriteRenderer();
	virtual bool Copy(const PScriptBase script) override;
	virtual void Load(const std::string* inputArg, int argS) override;
	virtual Matrix& GetTransform();
	virtual SSprite& GetSprite();
	virtual Vector3 GetPosition();
	virtual PDx9Texture GetTexture();
	virtual RECT GetSourceRect();
	virtual void Update() override;
protected:
	Matrix transformMatrix;
	SSprite sprite;
private:
	REGISTER_START(SpriteRenderer);
};
