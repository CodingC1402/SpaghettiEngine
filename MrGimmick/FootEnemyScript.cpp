#include "FootEnemyScript.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Polygon2DCollider.h"
#include "EnemyJumpScript.h"
#include "AIScript.h"

REGISTER_FINISH(FootEnemyScript, ScriptBase) {}

ScriptBase* FootEnemyScript::Clone() const
{
	auto clone = dynamic_cast<FootEnemyScript*>(ScriptBase::Clone());

	clone->_destroyDelay = _destroyDelay;
	clone->_diedFieldName = _diedFieldName;
	clone->_hurtVel = _hurtVel;

	return clone;
}

void FootEnemyScript::Died()
{
	_isDead.lock()->SetValue(true);
	_destroyCounter = _destroyDelay;

	GetGameObject()->GetScriptContainer().IteratingWithLamda(
		[&](PScriptBase script) {
			if (!(script == this || script == _animator || script == _rb))
				script->Disable();
		}
	);
	GetGameObject()->GetChildContainer().IteratingWithLamda(
		[&](PGameObj obj) {
			obj->Disable();
		}
	);
}

void FootEnemyScript::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
	_healthScript->AddToHealthEvent(
		[&](const int& health, const int& delta) {
			auto vel = _hurtVel;
			vel.x *= _moveScript->IsFlipped() ? 1 : -1;
			_rb->SetVelocity(vel);

			if (delta < 0 && health <= 0)
			{
				Died();
			}
		}
	);

	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_isDead = _animator->GetField<bool>(_diedFieldName);
}

void FootEnemyScript::OnUpdate()
{
	if (_destroyCounter > 0.0f)
	{
		_destroyCounter -= GameTimer::GetDeltaTime();
		if (_destroyCounter <= 0.0f)
		{
			_animator->Disable();
			GetGameObject()->CallDestroy();
		}
	}
}

void FootEnemyScript::Load(nlohmann::json& input)
{
	_diedFieldName = input[Fields::FootEnemyScript::_diedNode].get<std::string>();
	_destroyDelay = input[Fields::FootEnemyScript::_delay].get<float>();
	_hurtVel = input[Fields::FootEnemyScript::_hurtVel];
}
