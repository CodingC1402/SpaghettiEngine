#include "HealthScript.h"

REGISTER_FINISH(HealthScript, ScriptBase) {}

void HealthScript::Load(nlohmann::json& input)
{
}

void HealthScript::OnStart()
{
}

void HealthScript::OnCollide(CollideEvent& e)
{
}

PScriptBase HealthScript::Clone() const
{
	return PScriptBase();
}
