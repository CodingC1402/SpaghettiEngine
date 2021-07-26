#include "Pickups.h"
#include "FieldNames.h"

REGISTER_FINISH(Pickups, ScriptBase) {}

void Pickups::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Contain(Fields::SpecialTag::GetPlayerTag()) && !_used)
	{
		if (PlayerInventory::GetInstance()->AddItem(_type))
		{
			GetGameObject()->CallDestroy();
			_used = true;
		}
	}

	if (e.GetGameObject()->GetTag() != Fields::SpecialTag::GetPlatformTag())
	{
		e.SetIsHandled(true);
	}
}

NLOHMANN_JSON_SERIALIZE_ENUM(ItemSlotUI::Item, {
	{ItemSlotUI::Item::FireBall, "FireBall"},
	{ItemSlotUI::Item::Bomb, "Bomb"},
	{ItemSlotUI::Item::HealthPotion, "HealthPotion"}
	})
void Pickups::Load(nlohmann::json& input)
{
	_type = input["Type"];
}

ScriptBase* Pickups::Clone() const
{
	auto clone = dynamic_cast<Pickups*>(ScriptBase::Clone());

	clone->_type = _type;

	return clone;
}
