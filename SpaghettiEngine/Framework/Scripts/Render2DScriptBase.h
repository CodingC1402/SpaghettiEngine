#pragma  once
#include "ScriptBase.h"
#include "CornDirectX.h"
#include "Camera.h"

class Render2DScriptBase : public ScriptBase
{
public:
	Render2DScriptBase();

	virtual void Update() override;
	virtual void Draw(SpriteHandler handler, PCamera camera) {}
private:
	REGISTER_START(Render2DScriptBase);
};