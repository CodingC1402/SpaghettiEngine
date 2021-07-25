#include "PitfallScript.h"
#include "FieldNames.h"
#include "AttackScript.h"
#include "LoadingJson.h"

REGISTER_FINISH(PitfallScript, ScriptBase) {}

void PitfallScript::OnCollide(CollideEvent& e)
{
	if (e.GetEventOwner() != GetGameObject())
		return;

	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerAttack()))
	{
		auto script = dynamic_cast<AttackScript*>(e.GetGameObject()->GetScriptContainer().GetItemType(TYPE_NAME(AttackScript)));
		script->Explode();
	}
}

void PitfallScript::OnFixedUpdate()
{
	_collided.clear();
}