#include "RectangleUI.h"
#include "Graphics.h"

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

	_sprite = TextureContainer::GetInstance()->GetResource(input["Texture"].get<CULL>())->GetSprite(input["Index"].get<unsigned>());
}
