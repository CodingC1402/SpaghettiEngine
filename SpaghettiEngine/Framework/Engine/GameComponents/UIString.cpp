#include "UIString.h"
#include "UIFields.h"
#include "Graphics.h"

REGISTER_UI_COMPONENT_FINISH(UIString);

void UIString::Draw(Vector3 pos)
{
	auto _actualPos = pos + GetPosition();
	GetSprites();

	int _counter = _size < 0 ? 0 : _size - static_cast<int>(_text.size());
	_counter = _counter < 0 ? 0 : _counter;
	while (_counter > 0)
	{
		Graphics::DrawUI(_fillSprite, _actualPos, GetColor());
		_actualPos.x += _map->GetWidth() + _map->GetSpacing();
		_counter--;
	}

	for (auto sprite : _sprites)
	{
		Graphics::DrawUI(sprite, _actualPos, GetColor());
		_actualPos.x += _map->GetWidth() + _map->GetSpacing();

		_counter++;
		if (_size >= 0 && _counter > _size)
			break;
	}
	UIComponent::Draw(pos);
}

void UIString::Load(nlohmann::json& input)
{
	UIComponent::Load(input);
	_text = input[UIField::UIString::_text].get<std::string>();
	_fill = input[UIField::UIString::_fill].get<std::string>()[0];

	_size = input[UIField::UIString::_size].empty() ? -1 : input[UIField::UIString::_size].get<int>();
	_map = TextMapContainer::GetInstance()->GetResource(input[UIField::UIString::_textMap].get<CULL>());
	_fillSprite = _map->GetSprite(_fill);
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