#include "MoveScript.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "SMath.h"
#include "FieldNames.h"

REGISTER_FINISH(MoveScript, ScriptBase) {}

void MoveScript::Load(nlohmann::json& input)
{
	_jumpStrength = input[Fields::Player::_jumpStrengthField].get<float>();
	_speedCap =		input[Fields::Player::_speedCapField].get<float>();
	_speedRamUp =	input[Fields::Player::_speedRamUpField].get<float>();
	_reduceSpeed =	input[Fields::Player::_reduceSpeed].get<float>();

	_gsDropFactor =	input[Fields::Player::_gsDropFactor].get<float>();
	_minGravityScale = input[Fields::Player::_gsMin].get<float>();
}

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
	_moveVec = _rigidBody->GetVelocity();

	JumpAction();
	MoveAction();

	_rigidBody->SetVelocity(_moveVec);
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

bool MoveScript::IsFlipped() const noexcept
{
	return isFlipped;
}

void MoveScript::SetAllowJump(bool value) noexcept
{
	_isAllowJump = value;
}

bool MoveScript::IsAllowJump() const noexcept
{
	return _isAllowJump;
}

void MoveScript::JumpAction()
{
	if (_isJumping && _jumpInput->CheckKeyRelease())
		ResetJumpAction();

	if (_isJumping)
	{
		if (_gravityScale <= _minGravityScale)
			ResetJumpAction();
		else
		{
			_gravityScale -= _gravityScale * _gsDropFactor * GameTimer::GetDeltaTime();
			_rigidBody->SetGravityScale(_gravityScale);
		}
	}

	if (_isGrounded && _isAllowJump && _jumpInput->CheckKeyPress())
	{
		_isJumping = true;
		_gravityScale = _rigidBody->GetGravityScale();
		_baseGravityScale = _gravityScale;

		_moveVec.y = _jumpStrength;
	}
}

void MoveScript::ResetJumpAction()
{
	if (!_isJumping)
		return;

	_isJumping = false;
	_rigidBody->SetGravityScale(_baseGravityScale);
}

void MoveScript::MoveAction()
{
	float totalVel = 0.0f;
	CheckDirection(_leftInput->CheckKeyRelease(), _leftInput->CheckKeyDown(), -1, _isWalkingLeft, totalVel);
	CheckDirection(_rightInput->CheckKeyRelease(), _rightInput->CheckKeyDown(), 1, _isWalkingRight, totalVel);

	totalVel *= _speedRamUp * GameTimer::GetDeltaTime();
	if (_moveVec.x * totalVel < 0.0f)
		_moveVec.x += totalVel;
	else if (abs(_moveVec.x) < _speedCap)
		_moveVec.x += totalVel;

	_isRunningField.lock()->SetValue(IsWalking());
}

void MoveScript::CheckDirection(const bool& keyRelease, const bool& keyDown, const int& factor, bool& directionMove, float& totalVel) noexcept
{
	if (keyDown)
	{
		totalVel = factor;
		if (isFlipped != (factor < 0))
		{
			GetGameObject()->GetTransform().SetScale(factor, 1, 1);
			isFlipped = (factor < 0);
		}
		directionMove = true;
	}
	else if (keyRelease)
	{
		directionMove = false;
		if (_moveVec.x * factor > 0) // Moving same direction
		{
			_moveVec.x -= factor * _reduceSpeed;
			_moveVec.x = _moveVec.x * factor < 0 ? 0 : _moveVec.x;
		}
	}
}

bool MoveScript::IsWalking() const noexcept
{
	return _isWalkingLeft ^ _isWalkingRight;
}

PScriptBase MoveScript::Clone() const
{
	auto clone = dynamic_cast<MoveScript*>(ScriptBase::Clone());

	clone->_speedCap = _speedCap;
	clone->_jumpStrength = _jumpStrength;
	clone->_speedRamUp = _speedRamUp;

	clone->_moveVec = _moveVec;
	clone->isFlipped = isFlipped;
	clone->_isAllowJump = _isAllowJump;

	return clone;
}
