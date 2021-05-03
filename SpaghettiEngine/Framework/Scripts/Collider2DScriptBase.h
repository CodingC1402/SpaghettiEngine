#pragma once
#include "ScriptBase.h"
#include "CornDirectX.h"
#include "Camera.h"

class Collider2DScriptBase : public ScriptBase
{
public:
	Collider2DScriptBase();

	virtual void Update() override;
	virtual void DrawBox(RenderDevice render, PCamera camera, Color color) {}
private:
	REGISTER_START(Collider2DScriptBase);
};