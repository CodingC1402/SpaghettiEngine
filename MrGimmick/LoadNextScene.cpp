#include "LoadNextScene.h"
#include "FieldNames.h"
#include "SceneManager.h"

REGISTER_FINISH(LoadNextScene, ScriptBase) {}

void LoadNextScene::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		SceneManager::CallLoadNextScene();
	}
}
