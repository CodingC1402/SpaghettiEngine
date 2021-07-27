#include "Pickups.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(Pickups, ScriptBase) {}

void Pickups::OnUpdate()
{
	_time -= GameTimer::GetDeltaTime();
	if (_time <= 0)
		GetGameObject()->CallDestroy();
}

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
	_time = input["Time"];
}

void Pickups::SetTime(unsigned time)
{
	_time = time;
}

ScriptBase* Pickups::Clone() const
{
	auto clone = dynamic_cast<Pickups*>(ScriptBase::Clone());

	clone->_type = _type;
	clone->_time = _time;

	return clone;
}
