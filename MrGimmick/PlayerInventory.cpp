#include "PlayerInventory.h"
#include "LoadingJson.h"
#include "Canvas.h"

REGISTER_FINISH(PlayerInventory, ScriptBase) {}

void PlayerInventory::OnEnabled()
{
	if (!__instance)
		__instance = this;
}

void PlayerInventory::OnDisabled()
{
	if (__instance == this)
		__instance = nullptr;
}

void PlayerInventory::OnStart()
{
	auto canvas = GET_FIRST_SCRIPT_OF_TYPE(Canvas);
	_ui = dynamic_cast<ItemSlotUI*>(canvas->GetComponent("Inventory"));
}

NLOHMANN_JSON_SERIALIZE_ENUM(ItemSlotUI::Item, {
	{ItemSlotUI::Item::FireBall, "FireBall"},
	{ItemSlotUI::Item::Bomb, "Bomb"},
	{ItemSlotUI::Item::HealthPotion, "HealthPotion"}
	})
void PlayerInventory::Load(nlohmann::json& input)
{
	for (auto& obj : input[LoadingJson::Field::gameObjectsField])
	{
		_prefabs.emplace(
			obj["Type"].get<ItemSlotUI::Item>(),
			dynamic_cast<GameObj*>(GetOwner()->GetComponent(obj[LoadingJson::Field::idField].get<CULL>()))
		);
	}
}

GameObj* PlayerInventory::UseItem()
{
	auto type = _ui->GetFirstItem();
	if (type == ItemType::NoItem)
		return nullptr;
	return GetOwner()->Instantiate(_prefabs[type], Vector3());
}

bool PlayerInventory::AddItem(ItemType type)
{
	if (_ui->GetItemCount() >= 3)
		return false;
	else
	{
		_ui->AddItem(type);
		return true;
	}
}

void PlayerInventory::RemoveTopItem()
{
	_ui->UseFirstItem();
}

PlayerInventory* PlayerInventory::GetInstance()
{
	return __instance;
}
