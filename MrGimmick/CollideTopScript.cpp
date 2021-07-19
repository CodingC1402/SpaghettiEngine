#include "CollideTopScript.h"
#include "FieldNames.h"

REGISTER_FINISH(CollideTopScript, ScriptBase) {}

void CollideTopScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerAttack()) ||
		e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetEnemyTag()))
	{
		e.SetIsHandled(true);
	}

	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		if (e.GetNormal().y < 0.75)
			e.SetIsHandled(true);
	}
}
