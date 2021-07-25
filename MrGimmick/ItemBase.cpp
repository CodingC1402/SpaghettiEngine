#include "ItemBase.h"
#include "FieldNames.h"

REGISTER_FINISH(ItemBase, ScriptBase){}

void ItemBase::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag() != Fields::SpecialTag::GetPlatformTag())
	{
		if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
			GetGameObject()->CallDestroy();
		e.SetIsHandled(true);
	}
}
