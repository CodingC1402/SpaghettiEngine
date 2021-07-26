#include "ItemSlotUI.h"
#include "Graphics.h"
#include "Texture.h"

REGISTER_UI_COMPONENT_FINISH(ItemSlotUI);

void ItemSlotUI::Draw(Vector3 pos)
{
	auto actualPos = GetPosition() + pos;
	UpdateSprite();
	for (auto& sprite : _sprites)
	{
		Graphics::DrawUI(sprite, actualPos, GetColor());
		actualPos.x += _width + _spacing;
	}

	UIComponent::Draw(pos);
}

NLOHMANN_JSON_SERIALIZE_ENUM(ItemSlotUI::Item, {
	{ItemSlotUI::Item::FireBall, "FireBall"},
	{ItemSlotUI::Item::Bomb, "Bomb"},
	{ItemSlotUI::Item::HealthPotion, "HealthPotion"}
	})
void ItemSlotUI::Load(nlohmann::json& input)
{
	UIComponent::Load(input);
	constexpr auto itemsField		= "Items";
	constexpr auto itemField		= "Item";
	constexpr auto textureField		= "Texture";
	constexpr auto indexUseField	= "UseIndex";
	constexpr auto indexUnuseField	= "UnuseIndex";
	constexpr auto inventoryField	= "Inventory";
	constexpr auto spacingField		= "Spacing";
	constexpr auto widthField		= "Width";

	SSprite currentUseSprite;
	SSprite currentUnuseSprite;
	STexture texture;
	for (auto& item : input[itemsField])
	{
		texture = TextureContainer::GetInstance()->GetResource(item[textureField].get<CULL>());
		currentUseSprite = texture->GetSprite(item[indexUseField].get<unsigned>());
		currentUnuseSprite = texture->GetSprite(item[indexUnuseField].get<unsigned>());
		_itemSprites.emplace(item[itemField].get<ItemSlotUI::Item>(), std::pair(currentUseSprite, currentUnuseSprite));
	}

	for (auto& item : input[inventoryField])
	{
		_items.push_back(item.get<ItemSlotUI::Item>());
	}

	_width = input[widthField].get<unsigned>();
	_spacing = input[spacingField].get<unsigned>();
}

unsigned ItemSlotUI::GetItemCount() const
{
	return _items.size();
}

ItemSlotUI::Item ItemSlotUI::GetFirstItem() const
{
	return _items.front();
}

void ItemSlotUI::UseFirstItem()
{
	_needUpdateSprite = true;
	_items.pop_front();
}

void ItemSlotUI::AddItem(const Item& newItem)
{
	_items.push_back(newItem);
	_needUpdateSprite = true;
}

void ItemSlotUI::UpdateSprite()
{
	if (!_needUpdateSprite)
		return;
	_needUpdateSprite = false;

	_sprites.clear();
	auto counter = 0;
	for (auto& item : _items)
	{
		if (counter == 0)
			_sprites.push_back(_itemSprites[item].first);
		else
			_sprites.push_back(_itemSprites[item].second);
		counter++;
	}
}
