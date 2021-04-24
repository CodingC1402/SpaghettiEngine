#include "Render2DScriptBase.h"
#include "GraphicsMath.h"

REGISTER_FINISH(Render2DScriptBase);

Render2DScriptBase::Render2DScriptBase()
{
	name = TYPE_NAME(Render2DScriptBase);
}

void Render2DScriptBase::Update()
{
	Graphics::Draw(this);
}


