#include "MaxHealthPotion.h"
#include "PlayerScript.h"
#include "FieldNames.h"
#include "HealthScript.h"

REGISTER_FINISH(MaxHealthPotion, ScriptBase) {}

void MaxHealthPotion::OnCollide(CollideEvent& e)
{
	if (!_healthIncrease)
		return;

	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		auto script = dynamic_cast<HealthScript*>(e.GetGameObject()->GetScriptContainer().GetItemType(TYPE_NAME(HealthScript)));
		script->SetMaxHealth(script->GetMaxHealth() + _healthIncrease);
		script->SetHealth(script->GetMaxHealth());
		_healthIncrease = 0;
	}
}

void MaxHealthPotion::Load(nlohmann::json& input)
{
	_healthIncrease = input["HealthIncrease"].get<unsigned>();
}

ScriptBase* MaxHealthPotion::Clone() const
{
	auto clone = dynamic_cast<MaxHealthPotion*>(ScriptBase::Clone());

	clone->_healthIncrease = _healthIncrease;

	return clone;
}
