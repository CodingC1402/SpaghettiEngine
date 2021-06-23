#include "AttackMove.h"
#include "LoadingJson.h"

REGISTER_FINISH(AttackMove);

AttackMove::AttackMove(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void AttackMove::OnStart()
{
	_attackKey = InputSystem::GetInput("Attack");
}

void AttackMove::OnUpdate()
{
	if (_attackKey->Check())
	{
		GetOwner()->Instantiate(_starPrefab, GetWorldTransform());
	}
}

void AttackMove::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input["GameObjects"][0]["ID"].get<CULL>()));
}
