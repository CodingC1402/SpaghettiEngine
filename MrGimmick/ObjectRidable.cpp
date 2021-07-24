#include "ObjectRidable.h"
#include "FieldNames.h"

REGISTER_FINISH(ObjectRidable, RidableScript) {}

void ObjectRidable::OnFixedUpdate()
{
	if (_objects.empty() && __instance == this)
	{
		__instance = nullptr;
		return;
	}
	RidableScript::OnFixedUpdate();
}

void ObjectRidable::OnCollide(CollideEvent& e)
{
	if (e.GetEventOwner() != GetGameObject())
		return;

	if ((__instance && __instance != this) || e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetEnemyTag() | Fields::SpecialTag::GetPlayerAttack()))
	{
		e.SetIsHandled(true);
		return;
	}
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()) && e.GetNormal().y > 0.75f)
	{
		_objects.insert(e.GetGameObject());
		__instance = this;
	}
}

void ObjectRidable::OnDisabled()
{
	__instance = __instance == this ? nullptr : __instance;
}
