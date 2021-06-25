#include "MoveScript.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "SMath.h"
#include "FieldNames.h"
#include "FieldNames.h"

REGISTER_FINISH(MoveScript);

void MoveScript::Load(nlohmann::json& input)
{
	_jumpStrength = input[Fields::Player::_jumpStrengthField].get<float>();
	_speedCap = input[Fields::Player::_speedCapField].get<float>();
	_speedRamUp = input[Fields::Player::_speedRamUpField].get<float>();
}

MoveScript::MoveScript(PScene owner) : ScriptBase(owner)
{}

void MoveScript::OnStart()
{
	_rigidBody = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_baseGravityScale = _rigidBody->GetGravityScale();

	_isRunningField = _animator->GetField<bool>(Fields::Player::_isRunning);
	_isGroundedField = _animator->GetField<bool>(Fields::Player::_isGrounded);

	_leftInput  =		std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveLeft));
	_rightInput =		std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveRight));
	_jumpInput  =		std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_jump));
}

void MoveScript::OnUpdate()
{
	move.x = 0;
	move.y = 0;
	move.z = 0;

	if (_jumpInput->Check())
	{
		move.y += _jumpStrength;
	}
	if (_leftInput->Check())
	{
		move.x -= 1;
		if (!isFlipped)
		{
			GetGameObject()->GetTransform().SetScale(-1, 1, 1);
			isFlipped = true;
		}
	}
	if (_rightInput->Check())
	{
		move.x += 1;
		if (isFlipped)
		{
			GetGameObject()->GetTransform().SetScale(1, 1, 1);
			isFlipped = false;
		}
	}

	move.x *= _speedRamUp * GameTimer::GetDeltaTime();

	Vector3 velocity = _rigidBody->GetVelocity();
	if (move.x * velocity.x < 0)
		velocity.x += move.x;
	else if (abs(velocity.x) < _speedCap)
		velocity.x += move.x;
	velocity.y += move.y;
	_rigidBody->SetVelocity(velocity);
}

void MoveScript::OnCollideEnter(CollideEvent& e)
{
}

void MoveScript::OnCollideExit(CollideEvent& e)
{
}

void MoveScript::SetGrounded(bool value) noexcept
{
	_isGrounded = value;
	_isGroundedField.lock()->SetValue(_isGrounded);
}

bool MoveScript::GetGrounded() const noexcept
{
	return _isGrounded;
}

void MoveScript::JumpAction()
{
	if (_isJumping)
	{
		_rigidBody-
	}

	if (_isGrounded && _jumpInput->CheckKeyPress())
	{
		_isJumping = true;
	}
	else if (!_isGrounded && _jumpInput->CheckKeyRelease())
	{
		_isJumping = false;
	}
}

void MoveScript::MoveAction()
{
}

PScriptBase MoveScript::Clone() const
{
	auto clone = dynamic_cast<MoveScript*>(ScriptBase::Clone());

	clone->_speedCap = _speedCap;
	clone->_jumpStrength = _jumpStrength;
	clone->_speedRamUp = _speedRamUp;

	clone->move = move;
	clone->isFlipped = isFlipped;

	return clone;
}
