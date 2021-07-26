#include "BombScript.h"
#include "DeadAnimationScript.h"
#include "LoadingJson.h"

REGISTER_FINISH(BombScript, SpecialAttackScript) {}

void BombScript::OnStart()
{
	SpecialAttackScript::OnStart();
	GetAttackScript()->AddExplodeEvent(
		[&](){
			Explode();
		}
	);
}

void BombScript::Explode()
{
	auto instance = GetOwner()->Instantiate(_prefab, GetWorldTransform());
	auto anim = dynamic_cast<DeadAnimationScript*>(instance->GetScriptContainer().GetItemType(TYPE_NAME(DeadAnimationScript)));
	anim->Start();
}

void BombScript::Load(nlohmann::json& input)
{
	_prefab = GET_REF_OBJECT(0);
}

ScriptBase* BombScript::Clone() const
{
	auto clone = dynamic_cast<BombScript*>(SpecialAttackScript::Clone());

	clone->_prefab = _prefab;

	return clone;
}
