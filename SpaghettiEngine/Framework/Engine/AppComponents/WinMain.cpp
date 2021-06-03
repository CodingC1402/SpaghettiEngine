#include "WinMain.h"
#include "App.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "TileMapRenderer.h"
#include "CircleCollider.h"
#include "LineRenderBase.h"
#include "RigidBody2D.h"
#include "Polygon2DCollider.h"

BOOL WinMain::Start(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow)
{
	PApp app = App::GetInstance();
	//app->ShowExtraInfo();
	BOOL result = app->Go();
	return result;
}

void WinMain::RegisterSpaghettiScripts()
{
	ScriptBase* dummyScript;

	// This is dumb : ^)
	dummyScript = new Camera(nullptr);
	dummyScript = new SpriteRenderer(nullptr);
	dummyScript = new Animator(nullptr);
	dummyScript = new TileMapRenderer(nullptr);
	dummyScript = new LineRendererBase(nullptr);
	dummyScript = new CircleCollider(nullptr);
	dummyScript = new RigidBody2D(nullptr);
	dummyScript = new Polygon2DCollider(nullptr);
}
