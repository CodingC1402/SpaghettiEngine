#include "CollideTopScript.h"
#include "FieldNames.h"

REGISTER_FINISH(CollideTopScript, ScriptBase) {}

void CollideTopScript::OnCollide(CollideEvent& e)
{
	if (e.GetEventOwner() != GetGameObject())
		return;

	if (e.GetGameObject()->GetTag().Collide(
		Fields::SpecialTag::GetPlayerAttack() | 
		Fields::SpecialTag::GetEnemyTag() | 
		Fields::SpecialTag::GetStaticEnemyTag()))
	{
		e.SetIsHandled(true);
	}

	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		if (e.GetNormal().y <= 0.25)
			e.SetIsHandled(true);
	}
}
