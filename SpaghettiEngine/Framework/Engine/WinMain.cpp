#include "WinMain.h"
#include "App.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "TileMapRenderer.h"
#include "TileMapCollider2D.h"
#include "RigidBody2D.h"
#include "BoxCollider2D.h"
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
	dummyScript = new RigidBody2D(nullptr);
	dummyScript = new BoxCollider2D(nullptr);
	dummyScript = new TileMapCollider2D(nullptr);
}
