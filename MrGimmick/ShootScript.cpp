#include "ShootScript.h"
#include "LoadingJson.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(ShootScript, ScriptBase) {}

void ShootScript::OnStart()
{
	_sound = GET_FIRST_SCRIPT_OF_TYPE(CanonSound);
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_isReloadedField = _animator->GetField<bool>("IsReloaded");
}

void ShootScript::OnUpdate()
{
	if (_currentTime >= _reloadTime - 0.2 && _currentTime < _reloadTime)
	{
		_isReloadedField.lock()->SetValue(true);
	}
	if (_currentTime >= _reloadTime)
	{
		_sound->PlayShootSound();

		auto ball = GetOwner()->Instantiate(_starPrefab, GetWorldTransform() + _appearOffSet);
		auto _canonBallScript = dynamic_cast<CanonBallScript*>(ball->GetScriptContainer().GetItemType(TYPE_NAME(CanonBallScript)));
		_firedCanonBalls.push_back(std::dynamic_pointer_cast<GameObj>(ball->GetSharedPtr()));

		_canonBallScript->Throw(_rb->GetVelocity(), isFliped);

		_currentTime = 0;
		_isReloadedField.lock()->SetValue(false);
	}

	_currentTime += GameTimer::GetDeltaTime();

}

void ShootScript::OnFixedUpdate()
{
	for (auto it = _firedCanonBalls.begin(); it != _firedCanonBalls.end();)
	{
		if (it->expired())
			it = _firedCanonBalls.erase(it);
		else
			++it;
	}
}

void ShootScript::OnDisabled()
{
	for (auto& obj : _firedCanonBalls)
		if (!obj.expired())
			obj.lock()->CallDestroy();
}

void ShootScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Contain(Tag("Enemy")))
		e.SetIsHandled(true);
}

ScriptBase* ShootScript::Clone() const
{
	auto clone = dynamic_cast<ShootScript*>(ScriptBase::Clone());

	clone->_starPrefab = _starPrefab;
	clone->_reloadTime = _reloadTime;
	clone->_appearOffSet = _appearOffSet;

	return clone;
}

void ShootScript::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
	_appearOffSet = input[Fields::Player::_appearOffSet];
	_reloadTime = input["ReloadTime"];
}