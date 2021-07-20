#include "ObjectRidable.h"
#include "FieldNames.h"

REGISTER_FINISH(ObjectRidable, RidableScript) {}

void ObjectRidable::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()) && e.GetNormal().y > 0.75f)
		_objects.insert(e.GetGameObject());
}
