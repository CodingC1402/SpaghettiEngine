#include "AttackMove.h"
#include "LoadingJson.h"
#include "FieldNames.h"

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
		auto star = GetOwner()->Instantiate(_starPrefab, GetWorldTransform() + _appearOffSet);
		star->SetParent(GetGameObject());
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
	auto offSet = input[Fields::Player::_appearOffSet];
	_appearOffSet.x = offSet[0].get<float>();
	_appearOffSet.y = offSet[1].get<float>();
	_appearOffSet.z = offSet[2].get<float>();
}
