#include "Collider2DScriptBase.h"
#include "Graphics.h"

REGISTER_FINISH(Collider2DScriptBase);

Collider2DScriptBase::Collider2DScriptBase()
{
	name = TYPE_NAME(Collider2DScriptBase);
}

void Collider2DScriptBase::Update()
{
	Graphics::Box(this);
}
