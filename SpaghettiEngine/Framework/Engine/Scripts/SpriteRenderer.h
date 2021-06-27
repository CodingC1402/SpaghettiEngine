#pragma once
#include "Sprite.h"
#include "Render2DScriptBase.h"

class SpriteRenderer : public Render2DScriptBase
{
	friend class Graphics;
public:
	SpriteRenderer(PScene owner, bool isDisabled = false);
	
	[[nodiscard]] virtual Matrix4		GetSpriteMatrix()	const noexcept;
	[[nodiscard]] virtual Vector3		GetCenter()		const noexcept;
	[[nodiscard]] virtual SSprite		GetSprite()		const noexcept;
	[[nodiscard]] virtual PTexture		GetTexture()	const noexcept;
	[[nodiscard]] virtual RECT			GetSourceRect() const noexcept;
	
	virtual void Draw(PCamera camera) override;
	virtual void Load(nlohmann::json& inputObject) override;

	PScriptBase Clone() const override;
protected:
	Matrix4 transformMatrix;
	SSprite sprite;
private:
	REGISTER_START(SpriteRenderer);
};
