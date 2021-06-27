#include "StarScript.h"
#include "GameTimer.h"
#include "FieldNames.h"

REGISTER_FINISH(StarScript);

StarScript::StarScript(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void StarScript::OnStart()
{
	_rbBody = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
}

void StarScript::OnUpdate()
{
	if (_counterStarted)
	{
		_counter += GameTimer::GetDeltaTime();
		if (_counter >= _explodeTime)
		{
			_explodedField.lock()->SetValue(true);
			if (_counter >= _explodeTime + _animExplodeTime)
				GetGameObject()->CallDestroy();
		}
	}
}

void StarScript::Load(nlohmann::json& input)
{
	_explodeTime = input[Fields::Star::_explodeTime].get<float>();
	_animExplodeTime = input[Fields::Star::_explodeAnimTime].get<float>();
	_beforeUsable = input[Fields::Star::_beforeUsable].get<float>();

	auto& startVel = input[Fields::Star::_startVelocity];
	_startVelocity.x = startVel[0].get<float>();
	_startVelocity.y = startVel[1].get<float>();
}

void StarScript::Throw(const Vector3& _playerVel)
{
	GetGameObject()->SetParent(nullptr);
	if (_counter < _beforeUsable || !_counterStarted)
	{
		GetGameObject()->CallDestroy();
		return;
	}

	_counterStarted = true;
	_rbBody->Enable();
	_rbBody->SetVelocity(_startVelocity + _playerVel);
}

void StarScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag() == "Player")
		e.SetIsHandled(true);
}

PScriptBase StarScript::Clone() const
{
	auto clone = dynamic_cast<StarScript*>(ScriptBase::Clone());

	clone->_beforeUsable	= _beforeUsable;
	clone->_counterStarted	= _counterStarted;
	clone->_counter			= _counter;
	clone->_explodeTime		= _explodeTime ; // In second
	clone->_animExplodeTime = _animExplodeTime;

	return clone;
}

void StarScript::SetCreated()
{
	_animator->Enable();
	_counterStarted = true;
}
