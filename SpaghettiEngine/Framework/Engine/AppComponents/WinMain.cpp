#include "WinMain.h"
#include "App.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "TileMapRenderer.h"
#include "CircleCollider.h"
#include "RigidBody2D.h"
#include "Polygon2DCollider.h"
#include "AudioPlayer.h"

#include "AnimationNode.h"
#include "TrueFalseLeafNode.h"

#include "AnimationTree.h"
#include "BTs.h"

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
	Node* dummyNode;
	BTs* dummyTree;

	// This is dumb : ^)
	/// I think this is some sort of optimization the compiler do
	/// cause why would it compile something that doesn't seem
	/// to be used? Well I don't know any more.
	dummyScript = new Camera(nullptr);
	dummyScript = new SpriteRenderer(nullptr);
	dummyScript = new Animator(nullptr);
	dummyScript = new TileMapRenderer(nullptr);
	dummyScript = new CircleCollider(nullptr);
	dummyScript = new RigidBody2D(nullptr);
	dummyScript = new Polygon2DCollider(nullptr);
	dummyScript = new AudioPlayer(nullptr);

	dummyNode = new AnimationNode();
	dummyNode = new TrueLeaf();
	dummyNode = new FalseLeaf();

	dummyTree = new BTs();
	dummyTree = new AnimationTree();
}
