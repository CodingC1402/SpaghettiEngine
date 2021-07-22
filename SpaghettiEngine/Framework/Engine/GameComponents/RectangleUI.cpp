#include "RectangleUI.h"
#include "Graphics.h"
#include "UIFields.h"

REGISTER_UI_COMPONENT_FINISH(RectangleUI);

void RectangleUI::Draw(Vector3 pos)
{
	auto _actualPos = pos + GetPosition();

	Graphics::DrawUI(_sprite, _actualPos, GetColor());
	UIComponent::Draw(pos);
}

void RectangleUI::Load(nlohmann::json& input)
{
	UIComponent::Load(input);
	_sprite = TextureContainer::GetInstance()->GetResource(
		input[UIField::RectangleUI::_texture].get<CULL>())->GetSprite(input[UIField::RectangleUI::_index].get<unsigned>());
}