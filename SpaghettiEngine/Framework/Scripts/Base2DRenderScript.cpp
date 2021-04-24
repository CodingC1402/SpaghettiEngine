#include "Base2DRenderScript.h"
#include "GraphicsMath.h"

REGISTER_FINISH(Base2DRenderScript);

Base2DRenderScript::Base2DRenderScript()
{
	name = TYPE_NAME(Base2DRenderScript);
}

Matrix Base2DRenderScript::GetWorldMatrix()
{
	return owner->GetWorldMatrix();
}

Vector3 Base2DRenderScript::GetPosition() const noexcept
{
	return owner->GetWorldTransform();
}


