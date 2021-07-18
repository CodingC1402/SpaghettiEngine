#include "ShootScript.h"
#include "LoadingJson.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(ShootScript, ScriptBase) {}

void ShootScript::OnStart()
{
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_isReloadedField = _animator->GetField<bool>("IsReloaded");
}

void ShootScript::OnUpdate()
{
	if (_canonBallScriptList.empty())
	{
		auto ball = GetOwner()->Instantiate(_starPrefab, GetWorldTransform() + _appearOffSet);
		ball->SetParent(GetGameObject());

		//_currentBall = std::dynamic_pointer_cast<GameObj>(ball->GetSharedPtr());

		auto _canonBallScript = dynamic_cast<CanonBallScript*>(ball->GetScriptContainer().GetItemType(TYPE_NAME(CanonBallScript)));
		_canonBallScriptList.push_back(_canonBallScript);
	}
	if (_currentTime >= _reloadTime - 0.2 && _currentTime < _reloadTime)
	{
		_isReloadedField.lock()->SetValue(true);
	}
	if (_currentTime >= _reloadTime && !_canonBallScriptList.empty())
	{
		//_canonBallScriptList.push_back(_canonBallScript);
		_canonBallScriptList.front()->Throw(_rb->GetVelocity(), isFliped);
		_canonBallScriptList.pop_front();
		//_canonBallScript->Throw(_rb->GetVelocity(), isFliped);

		//_canonBallScript = nullptr;
		_currentTime = 0;
		_isReloadedField.lock()->SetValue(false);
	}

	_currentTime += GameTimer::GetDeltaTime();

}

void ShootScript::OnDisabled()
{
	//if (_canonBallScript)
	//	_canonBallScript->Throw(_rb->GetVelocity(), isFliped);
	//_canonBallScript = nullptr;
	//if (!_currentBall.expired())
	//	_currentBall.lock()->CallDestroy();
}

void ShootScript::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
	auto offSet = input[Fields::Player::_appearOffSet];
	_appearOffSet.x = offSet[0].get<float>();
	_appearOffSet.y = offSet[1].get<float>();
	_appearOffSet.z = offSet[2].get<float>();
}