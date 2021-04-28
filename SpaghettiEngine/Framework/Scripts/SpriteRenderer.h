#pragma once
#include "Sprite.h"
#include "Render2DScriptBase.h"

class SpriteRenderer : public Render2DScriptBase
{
	friend class Graphics;
public:
	SpriteRenderer();
	
	[[nodiscard]] virtual Matrix		GetSpriteMatrix()	const noexcept;
	[[nodiscard]] virtual Vector3		GetCenter()		const noexcept;
	[[nodiscard]] virtual SSprite		GetSprite()		const noexcept;
	[[nodiscard]] virtual PDx9Texture	GetTexture()	const noexcept;
	[[nodiscard]] virtual RECT			GetSourceRect() const noexcept;
	
	virtual bool Copy(CPScriptBase script) override;
	virtual void Draw(SpriteHandler handler, PCamera camera) override;
protected:
	virtual void Load(nlohmann::json& inputObject) override;
protected:
	Matrix transformMatrix;
	SSprite sprite;
private:
	REGISTER_START(SpriteRenderer);
};
