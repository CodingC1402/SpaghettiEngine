#pragma once
#include "ScriptBase.h"
#include "ItemSlotUI.h"
#include <vector>

class PlayerInventory : public ScriptBase
{
public:
	using  ItemType = ItemSlotUI::Item;
public:
	void OnEnabled();
	void OnDisabled();
	void OnStart() override;
	void Load(nlohmann::json& input) override;

	// Use first item in the list
	GameObj* UseItem();
	// Return false if the inventory is full
	bool AddItem(ItemType type);

	static PlayerInventory* GetInstance();
private:
	std::map<ItemType, GameObj*> _prefabs;
	ItemSlotUI* _ui = nullptr;
	unsigned maxItemSize = 3;

	static inline PlayerInventory* __instance = nullptr;
	REGISTER_START(PlayerInventory);
};

