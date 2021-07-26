#include "AttackScript.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "DebugRenderer.h"
#include "Setting.h"
#include "PlayerScript.h"
#include "Graphics.h"
#include "PhysicCollide.h"
#include "PlayerSound.h"

constexpr unsigned DEBUG_COLOR = 0xFFFF0000;
REGISTER_FINISH(AttackScript, ScriptBase) {}

void AttackScript::OnStart()
{
	_sound			= GET_FIRST_SCRIPT_OF_TYPE(StarSound);
	_polyCollider	= GET_FIRST_SCRIPT_OF_TYPE(Polygon2DCollider);
	_rbBody			= GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);

	_animator		= GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_explodedField	= _animator->GetField<bool>("IsExploded");
}

void AttackScript::OnUpdate()
{
	if (_countUsable)
	{
		_usableCounter += GameTimer::GetDeltaTime();
		return;
	}

	if (_counterStarted)
	{
		_counter += GameTimer::GetDeltaTime();

		auto vel = _rbBody->GetVelocity();
		vel.x += _additionVel * GameTimer::GetDeltaTime() * (vel.x < 0 ? -1 : 1);
		_rbBody->SetVelocity(vel);

		auto camera = Graphics::GetActiveCamera();
		auto distance = !camera ? _maxDistance  : (GetWorldTransform() - camera->GetWorldTransform()).GetMagnitude();
		if (_exploded)
		{
			if (_counter > _animExplodeTime)
				GetGameObject()->CallDestroy();
		}
		else if (distance > _maxDistance || _counter > _explodeTime) 
		{
			Explode();
		}
	}
}

void AttackScript::OnFixedUpdate()
{
	if (_counterStarted)
	{
		std::set<GameObj*> objList;
		PhysicCollide::GetCollidedWithRectangle(
			GetGameObject(),
			objList,
			_cancelZone.GetCenter(),
			_cancelZone.GetWidth(),
			_cancelZone.GetHeight(),
			Fields::SpecialTag::GetPlatformTag(),
			PhysicCollide::FilterMode::Equal
		);
		if (!objList.empty())
			Explode();
	}

	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangleFromCenter(
			_cancelZone.GetCenter(),
			_cancelZone.GetWidth(),
			_cancelZone.GetHeight(),
			GetWorldMatrix(),
			DEBUG_COLOR
		);
	}
}

void AttackScript::Explode()
{
	if (_exploded)
		return;

	_sound->PlayBounceSound();
	_exploded = true;
	_counter = 0;
	_explodedField.lock()->SetValue(true);
	_rbBody->Disable();
	_polyCollider->Disable();
	// disable all child. 
	GetGameObject()->GetChildContainer().DisableAll();
	for (auto& fun : _explodeDelegate)
		fun();
}

void AttackScript::Load(nlohmann::json& input)
{
	_explodeTime = input[Fields::Star::_explodeTime].get<float>();
	_animExplodeTime = input[Fields::Star::_explodeAnimTime].get<float>();
	_beforeUsable = input[Fields::Star::_beforeUsable].get<float>();
	_maxDistance = input[Fields::Star::_maxDistance].get<float>();
	_cancelZone = input[Fields::Star::_cancelZone];
	_additionVel = input[Fields::Star::_addSpeed].get<float>();

	auto& startVel = input[Fields::Star::_startVelocity];
	_startVelocity.x = startVel[0].get<float>();
	_startVelocity.y = startVel[1].get<float>();
}

void AttackScript::Throw(const Vector3& _playerVel, bool isFliped)
{
	if (_usableCounter < _beforeUsable)
	{
		GetGameObject()->CallDestroy();
		for (auto& fun : _throwedDelegate)
			fun(true);
		return;
	}
	
	PlayerSound::GetCurrentPlayerSound()->PlayAttackSound();
	GetGameObject()->BecomeRootObject();
	
	_counterStarted = true;
	_countUsable = false;

	_polyCollider->Enable();
	_rbBody->Enable();

	Vector3 throwVel = _startVelocity;
	throwVel.x *= isFliped ? -1 : 1;
	throwVel += _playerVel;

	_rbBody->SetVelocity(throwVel);

	for (auto& fun : _throwedDelegate)
		fun(false);
}

void AttackScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetCharacterTag()))
		e.SetIsHandled(true);

	if (e.GetGameObject()->GetTag() == Fields::SpecialTag::GetPlatformTag())
		_sound->PlayBounceSound();
}

PScriptBase AttackScript::Clone() const
{
	auto clone = dynamic_cast<AttackScript*>(ScriptBase::Clone());

	clone->_beforeUsable	= _beforeUsable;
	clone->_usableCounter	= _usableCounter;
	clone->_counterStarted	= _counterStarted;
	clone->_counter			= _counter;
	clone->_explodeTime		= _explodeTime ; // In second
	clone->_animExplodeTime = _animExplodeTime;
	clone->_startVelocity	= _startVelocity;
	clone->_maxDistance		= _maxDistance;
	clone->_additionVel		= _additionVel;

	return clone;
}

void AttackScript::AddEvent(const std::function<void(bool)>& newEvent)
{
	_throwedDelegate.push_back(newEvent);
}

void AttackScript::AddExplodeEvent(const std::function<void()>& newEvent)
{
	_explodeDelegate.push_back(newEvent);
}

void AttackScript::ClearEvent()
{
	_throwedDelegate.clear();
}

void AttackScript::SetCreated()
{
	_countUsable = true;
	_animator->Enable();
}
