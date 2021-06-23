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
		_owner->Instantiate(_starPrefab.get(), GetWorldTransform());
	}
}

void AttackMove::Load(nlohmann::json& input)
{
	_starPrefab = std::dynamic_pointer_cast<GameObj>(_owner->GetComponent(input["GameObjects"][0]["ID"].get<CULL>()));
}
