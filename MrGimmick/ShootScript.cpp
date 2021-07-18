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

		auto _canonBallScript = dynamic_cast<CanonBallScript*>(ball->GetScriptContainer().GetItemType(TYPE_NAME(CanonBallScript)));
		_canonBallScriptList.push_back(_canonBallScript);
	}
	if (_currentTime >= _reloadTime - 0.2 && _currentTime < _reloadTime)
	{
		_isReloadedField.lock()->SetValue(true);
	}
	if (_currentTime >= _reloadTime && !_canonBallScriptList.empty())
	{
		_canonBallScriptList.front()->Throw(_rb->GetVelocity(), isFliped);
		_canonBallScriptList.pop_front();

		_currentTime = 0;
		_isReloadedField.lock()->SetValue(false);
	}

	_currentTime += GameTimer::GetDeltaTime();

}

void ShootScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Contain(Tag("Enemy")))
		e.SetIsHandled(true);
}

void ShootScript::OnDisabled()
{
	if (!_canonBallScriptList.empty())
		_canonBallScriptList.clear();
}

void ShootScript::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
	auto offSet = input[Fields::Player::_appearOffSet];
	_appearOffSet.x = offSet[0].get<float>();
	_appearOffSet.y = offSet[1].get<float>();
	_appearOffSet.z = offSet[2].get<float>();
}