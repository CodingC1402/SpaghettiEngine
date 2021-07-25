#include "AutoCallDestroy.h"
#include "GameTimer.h"

REGISTER_FINISH(AutoCallDestroy, ScriptBase) {}

void AutoCallDestroy::OnUpdate()
{
	_delay -= GameTimer::GetDeltaTime();
	if (_delay <= 0)
		GetGameObject()->CallDestroy();
}

void AutoCallDestroy::Load(nlohmann::json& input)
{
	_delay = input["Delay"].get<float>();
}

ScriptBase* AutoCallDestroy::Clone() const
{
	auto clone = dynamic_cast<AutoCallDestroy*>(ScriptBase::Clone());

	clone->_delay = _delay;

	return clone;
}
