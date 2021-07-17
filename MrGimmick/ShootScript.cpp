#include "ShootScript.h"
#include "LoadingJson.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(ShootScript, ScriptBase) {}

void ShootScript::OnStart()
{
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
}

void ShootScript::OnUpdate()
{
	if (_currentBall.expired())
	{
		auto ball = GetOwner()->Instantiate(_starPrefab, GetWorldTransform() + _appearOffSet);
		ball->SetParent(GetGameObject());

		_currentBall = std::dynamic_pointer_cast<GameObj>(ball->GetSharedPtr());
		_canonBallScript = dynamic_cast<CanonBallScript*>(ball->GetScriptContainer().GetItemType(TYPE_NAME(CanonBallScript)));

		_canonBallScript->Throw(_rb->GetVelocity(), isFliped);
		_canonBallScript = nullptr;
	}
}

void ShootScript::OnDisabled()
{
	if (_canonBallScript)
		_canonBallScript->Throw(_rb->GetVelocity(), isFliped);
	_canonBallScript = nullptr;
	if (!_currentBall.expired())
		_currentBall.lock()->CallDestroy();
}

void ShootScript::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
	auto offSet = input[Fields::Player::_appearOffSet];
	_appearOffSet.x = offSet[0].get<float>();
	_appearOffSet.y = offSet[1].get<float>();
	_appearOffSet.z = offSet[2].get<float>();
}