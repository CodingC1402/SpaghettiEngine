#pragma once
#include "UIComponent.h"
#include "Sprite.h"
#include <map>
#include <list>

class ItemSlotUI : public UIComponent
{
public:
	enum class Item
	{
		FireBall,
		Bomb,
		HealthPotion,
		NoItem
	};
public:
	void Draw(Vector3 pos) override;
	void Load(nlohmann::json& input) override;
	
	[[nodiscard]] unsigned GetItemCount() const;
	[[nodiscard]] Item GetFirstItem() const;
	void UseFirstItem();
	void AddItem(const Item& newItem);
private:
	void UpdateSprite();
private:
	std::list<Item> _items;
	bool _needUpdateSprite = true;

	// CurrentSprite reflection
	std::list<SSprite> _sprites;
	// All sprite;
	std::map<Item, std::pair<SSprite, SSprite>> _itemSprites;

	unsigned _spacing = 0;
	unsigned _width = 0;

	REGISTER_UI_COMPONENT_START(ItemSlotUI);
};

