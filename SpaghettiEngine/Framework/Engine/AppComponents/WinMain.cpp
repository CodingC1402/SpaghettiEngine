#include "WinMain.h"
#include "App.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "TileMapRenderer.h"
#include "CircleCollider.h"
#include "LineRenderBase.h"

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

	dummyScript = new Camera(nullptr);
	dummyScript = new SpriteRenderer(nullptr);
	dummyScript = new Animator(nullptr);
	dummyScript = new TileMapRenderer(nullptr);
	dummyScript = new LineRendererBase(nullptr);
	dummyScript = new CircleCollider(nullptr);
}
