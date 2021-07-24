#pragma once 
#include "UIComponent.h"
#include "Sprite.h"
#include "TextMap.h"
#include <map>

// Render string to sprite with predefined map

class UIString : public UIComponent
{
public:
	void Draw(Vector3 pos) override;
	void Load(nlohmann::json& input) override;

	void SetText(const std::string& text);
	[[nodiscard]] const std::string& GetText() const noexcept; 
private:
	void GetSprites();
private:
	// Update first time
	bool _textChanged = true;
	std::string _text = "";
	std::list<SSprite> _sprites;
	STextMap _map;

	REGISTER_UI_COMPONENT_START(UIString);
};