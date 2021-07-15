#pragma once
#include "UIComponent.h"
#include "Sprite.h"

class RectangleUI : public UIComponent
{
public:
	void Draw(Vector3 pos) override;
	void Load(nlohmann::json& input) override;
private:
	SSprite _sprite;

	REGISTER_UI_COMPONENT_START(RectangleUI);
};