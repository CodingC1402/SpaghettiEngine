#pragma  once
#include "ScriptBase.h"
#include "CornDirectX.h"
#include "Camera.h"

class Base2DRenderScript : ScriptBase
{
public:
	Base2DRenderScript();

	[[nodiscard]] virtual Matrix		GetWorldMatrix();
	[[nodiscard]] virtual Vector3		GetPosition()	const noexcept;

	virtual void Update() override;
	virtual void Draw(SpriteHandler handler, PCamera camera) = 0;
private:
	REGISTER_START(Base2DRenderScript);
};