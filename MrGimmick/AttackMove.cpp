#include "AttackMove.h"
#include "LoadingJson.h"

AttackMove::AttackMove(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void AttackMove::Load(nlohmann::json & input)
{
	using LoadingJson::Field;

	_starPrefab = std::dynamic_pointer_cast<GameObj>(_owner->GetComponent(input[Field::gameObjectsField].get<CULL>()));
}

void AttackMove::OnUpdate()
{
	
}
