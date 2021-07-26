#include "CanonBallScript.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "HealthScript.h"
#include "LootTable.h"

REGISTER_FINISH(CanonBallScript, ScriptBase) {}

void CanonBallScript::OnStart()
{
	_sound = GET_FIRST_SCRIPT_OF_TYPE(BaseEnemySound);
	_polyCollider = GET_FIRST_SCRIPT_OF_TYPE(Polygon2DCollider);
	//_cirCollider = GET_FIRST_SCRIPT_OF_TYPE(CircleCollider);
	_rbBody = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	auto healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
	healthScript->AddToHealthEvent(
		[&](const int& health, const int& delta) {
			if (health <= 0)
			{
				_destroyed = true;
				_counter = 0;
				LootTable::GetInstance()->SpawnRandom(GetWorldTransform());
			}
		}
	);

	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_explodedField = _animator->GetField<bool>("IsExploded");
}

void CanonBallScript::DisableBeforeExplode()
{
	_sound->PlayDeadSound();
	_explodedField.lock()->SetValue(true);
	_rbBody->Disable();
	_polyCollider->Disable();
	GetGameObject()->GetChildContainer().DisableAll();
}

void CanonBallScript::OnUpdate()
{
	if (_counterStarted)
	{
		_counter += GameTimer::GetDeltaTime();

		auto vel = _rbBody->GetVelocity();
		vel.x += _additionVel * GameTimer::GetDeltaTime() * (vel.x < 0 ? -1 : 1);
		_rbBody->SetVelocity(vel);

		if (_destroyed)
		{
			DisableBeforeExplode();
			if (_counter >= _animExplodeTime)
				GetGameObject()->CallDestroy();
		}
		else if (_counter >= _explodeTime)
		{
			DisableBeforeExplode();
			if (_counter >= _explodeTime + _animExplodeTime)
				GetGameObject()->CallDestroy();
		}
	}
}

void CanonBallScript::Load(nlohmann::json& input)
{
	_explodeTime = input[Fields::Star::_explodeTime].get<float>();
	_animExplodeTime = input[Fields::Star::_explodeAnimTime].get<float>();

	auto& startVel = input[Fields::Star::_startVelocity];
	_startVelocity.x = startVel[0].get<float>();
	_startVelocity.y = startVel[1].get<float>();
}

void CanonBallScript::Throw(const Vector3& _playerVel, bool isFliped)
{
	GetGameObject()->BecomeRootObject();

	_counterStarted = true;

	_polyCollider->Enable();
	_rbBody->Enable();
	_animator->Enable();

	Vector3 throwVel = _startVelocity;
	throwVel.x *= isFliped ? -1 : 1;
	throwVel += _playerVel;

	_rbBody->SetVelocity(throwVel);
}

void CanonBallScript::OnCollide(CollideEvent& e)
{
	//if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetCharacterTag()))
	//	e.SetIsHandled(true);
}

PScriptBase CanonBallScript::Clone() const
{
	auto clone = dynamic_cast<CanonBallScript*>(ScriptBase::Clone());

	clone->_counterStarted = _counterStarted;
	clone->_counter = _counter;
	clone->_explodeTime = _explodeTime; // In second
	clone->_animExplodeTime = _animExplodeTime;
	clone->_startVelocity = _startVelocity;

	return clone;
}