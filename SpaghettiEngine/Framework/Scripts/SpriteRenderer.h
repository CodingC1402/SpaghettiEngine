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
	virtual const PMatrix GetTransform();
	virtual const Vector3* GetPosition();
	virtual const Vector3* GetCenter();
	virtual SSprite& GetSprite();
	virtual PDx9Texture GetTexture();
	virtual const RECT* GetSourceRect();
	virtual void Update() override;
protected:
	virtual void Load(const std::string* inputArg) override;
protected:
	Matrix transformMatrix;
	SSprite sprite;
	Vector3 center;
private:
	REGISTER_START(SpriteRenderer);
};
