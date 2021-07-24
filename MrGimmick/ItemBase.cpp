#include "ItemBase.h"
#include "FieldNames.h"

REGISTER_FINISH(ItemBase, ScriptBase){}

void ItemBase::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag() != Fields::SpecialTag::GetPlatformTag())
	{
		e.SetIsHandled(true);
	}
}
