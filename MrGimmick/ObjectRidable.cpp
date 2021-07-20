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
	if (__instance && e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
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
