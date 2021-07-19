#include "StarRidableScript.h"
#include "FieldNames.h"

REGISTER_FINISH(StarRidableScript, ScriptBase) {}

void StarRidableScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerAttack()))
		e.SetIsHandled(true);
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		if (e.GetNormal().y < 0.75)
			e.SetIsHandled(true);
	}
}
