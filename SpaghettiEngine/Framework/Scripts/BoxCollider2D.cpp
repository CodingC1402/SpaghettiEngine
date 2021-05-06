#include "BoxCollider2D.h"

REGISTER_FINISH(BoxCollider2D);

BoxCollider2D::BoxCollider2D(PScene owner) : Collider2DScriptBase(owner)
{
	_name = TYPE_NAME(BoxCollider2D);
}

Vector3 BoxCollider2D::GetSize()
{
	return size;
}

void BoxCollider2D::SetSize(Vector3 s)
{
	size = s;
}

void BoxCollider2D::Load(nlohmann::json& input)
{
	
}
