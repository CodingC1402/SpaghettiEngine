#include "Render2DScriptBase.h"
#include "Graphics.h"

REGISTER_FINISH(Render2DScriptBase);

Render2DScriptBase::Render2DScriptBase()
{
	name = TYPE_NAME(Render2DScriptBase);
}

void Render2DScriptBase::OnEnabled()
{
	Graphics::AddRender2D(this);
}

void Render2DScriptBase::OnDisabled()
{
	Graphics::RemoveRender2D(this);
}

