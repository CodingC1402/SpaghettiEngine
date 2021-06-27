#include "AttackMove.h"
#include "LoadingJson.h"

REGISTER_FINISH(AttackMove);

AttackMove::AttackMove(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void AttackMove::OnStart()
{
	_attackKey = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput("Attack"));
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
}

void AttackMove::OnUpdate()
{
	if (_attackKey->CheckKeyPress())
	{
		auto star = GetOwner()->Instantiate(_starPrefab, GetWorldTransform());
		_starScript = dynamic_cast<StarScript*>(star->GetScriptContainer().GetItemType(TYPE_NAME(StarScript)));
	}
	if (_starScript && _attackKey->CheckKeyRelease())
	{
		_starScript->Throw(_rb->GetVelocity());
		_starScript = nullptr;
	}
}

void AttackMove::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
}
