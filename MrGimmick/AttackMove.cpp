#include "AttackMove.h"
#include "LoadingJson.h"
#include "FieldNames.h"

REGISTER_FINISH(AttackMove, ScriptBase) {}

void AttackMove::OnStart()
{
	_attackKey = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput("Attack"));
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
}

void AttackMove::OnUpdate()
{
	if (_attackKey->CheckKeyPress() && _currentStar.expired())
	{
		auto star = GetOwner()->Instantiate(_starPrefab, GetWorldTransform() + _appearOffSet);

		_currentStar = std::dynamic_pointer_cast<GameObj>(star->GetSharedPtr());
		_starScript = dynamic_cast<StarScript*>(star->GetScriptContainer().GetItemType(TYPE_NAME(StarScript)));
		_throwed = false;
	}
	if (_starScript && _attackKey->CheckKeyRelease())
	{
		_starScript->Throw(Vector3(), _moveScript->IsFlipped());
		_starScript = nullptr;
		_throwed = true;
	}
}

void AttackMove::OnLateUpdate()
{
	if (_currentStar.expired() || _throwed)
		return;
	_currentStar.lock()->GetTransform().SetWorldTransform(GetWorldTransform() + _appearOffSet);
}

void AttackMove::OnDisabled()
{
	if (_starScript)
		_starScript->Throw(_rb->GetVelocity(), _moveScript->IsFlipped());
	_starScript = nullptr;
	if (!_currentStar.expired())
		_currentStar.lock()->CallDestroy();
}

void AttackMove::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
	auto offSet = input[Fields::Player::_appearOffSet];
	_appearOffSet.x = offSet[0].get<float>();
	_appearOffSet.y = offSet[1].get<float>();
	_appearOffSet.z = offSet[2].get<float>();
}
