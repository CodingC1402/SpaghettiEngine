#pragma once
#include "ScriptBase.h"
#include "Graphics.h"
#include "Sprite.h"
#include "CornDirectX.h"
#include "Camera.h"

class Graphics;

class SpriteRenderer : public ScriptBase
{
	friend class Graphics;
public:
	SpriteRenderer();
	
	[[nodiscard]] virtual Matrix		GetWorldMatrix();
	[[nodiscard]] virtual Matrix		GetTransform()	const noexcept;
	[[nodiscard]] virtual Vector3		GetPosition()	const noexcept;
	[[nodiscard]] virtual Vector3		GetCenter()		const noexcept;
	[[nodiscard]] virtual SSprite		GetSprite()		const noexcept;
	[[nodiscard]] virtual PDx9Texture	GetTexture()	const noexcept;
	[[nodiscard]] virtual RECT			GetSourceRect() const noexcept;
	
	virtual bool Copy(const PScriptBase script) override;
	virtual void Update() override;
protected:
	virtual void Load(const std::string* inputArg) override;
protected:
	Matrix transformMatrix;
	SSprite sprite;
private:
	REGISTER_START(SpriteRenderer);
};
