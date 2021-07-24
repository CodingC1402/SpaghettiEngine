#include "UIString.h"
#include "UIFields.h"
#include "Graphics.h"

REGISTER_UI_COMPONENT_FINISH(UIString);

void UIString::Draw(Vector3 pos)
{
	auto _actualPos = pos + GetPosition();
	GetSprites();
	for (auto sprite : _sprites)
	{
		Graphics::DrawUI(sprite, _actualPos, GetColor());
		_actualPos.x += _map->GetWidth() + _map->GetSpacing();
	}
	UIComponent::Draw(pos);
}

void UIString::Load(nlohmann::json& input)
{
	UIComponent::Load(input);
	_text = input[UIField::UIString::_text].get<std::string>();
	_map = TextMapContainer::GetInstance()->GetResource(input[UIField::UIString::_textMap].get<CULL>());
}

void UIString::SetText(const std::string& text)
{
	if (text != _text)
	{
		_text = text;
		_textChanged = true;
	}
}

const std::string& UIString::GetText() const noexcept
{
	return _text;
}

void UIString::GetSprites()
{
	if (!_textChanged)
		return;

	_textChanged = false;
	_sprites.clear();
	for (auto& letter : _text)
	{
		_sprites.push_back(_map->GetSprite(letter));
	}
}