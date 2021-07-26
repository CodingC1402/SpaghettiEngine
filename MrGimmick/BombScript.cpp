#include "BombScript.h"
#include "DeadAnimationScript.h"

REGISTER_FINISH(BombScript, SpecialAttackScript) {}

void BombScript::OnStart()
{
	SpecialAttackScript::OnStart();
	GetAttackScript()->AddExplodeEvent(
		[&](){
			GameObj* obj = GetGameObject()->GetChildContainer().GetAllItemsWithName("Explosion").front();
			obj->Enable();
			auto script = dynamic_cast<DeadAnimationScript*>(obj->GetScriptContainer().GetItemType(TYPE_NAME(DeadAnimationScript)));
			script->Start();
		}
	);
}
