#pragma once
#include "Sprite.h"
#include "Render2DScriptBase.h"

class SpriteRenderer : public Render2DScriptBase
{
	friend class Graphics;
public:
	SpriteRenderer(PScene owner);
	
	[[nodiscard]] virtual Matrix		GetSpriteMatrix()	const noexcept;
	[[nodiscard]] virtual Vector3		GetCenter()		const noexcept;
	[[nodiscard]] virtual SSprite		GetSprite()		const noexcept;
	[[nodiscard]] virtual PImage		GetTexture()	const noexcept;
	[[nodiscard]] virtual RECT			GetSourceRect() const noexcept;
	
	virtual void Draw(SpriteHandler handler, PCamera camera) override;
	virtual void Load(nlohmann::json& inputObject) override;
	virtual BaseComponent* Clone() override;
protected:
	Matrix transformMatrix;
	SSprite sprite;
private:
	REGISTER_START(SpriteRenderer);
};
