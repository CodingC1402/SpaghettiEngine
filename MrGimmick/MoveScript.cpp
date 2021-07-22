#include "MoveScript.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "SMath.h"
#include "FieldNames.h"

REGISTER_FINISH(MoveScript, ScriptBase) {}

#pragma region MoveCommand
void MoveScript::StartMoveLeft() noexcept
{
	_isMoveLeftCommand = true;
}
void MoveScript::StartMoveRight() noexcept
{
	_isMoveRightCommand = true;
}
void MoveScript::StopMoveLeft() noexcept
{
	if (!_isMoveLeftCommand)
		return;
	_isMoveLeftCommand = false;
	StopMove(-1);
}
void MoveScript::StopMoveRight() noexcept
{
	if (!_isMoveRightCommand)
		return;
	_isMoveRightCommand = false;
	StopMove(1);
}
void MoveScript::StartJump() noexcept
{
	if (_isJumpCommand)
		return;
	_isJumpCommand = true;

	auto moveVec = _rigidBody->GetVelocity();
	if (_isGrounded && _isAllowJump)
	{
		_isJumping = true;
		_gravityScale = _rigidBody->GetGravityScale();
		_baseGravityScale = _gravityScale;

		moveVec.y = _jumpStrength;
	}
	_rigidBody->SetVelocity(moveVec);
}
void MoveScript::StopJump() noexcept
{
	if (!_isJumpCommand)
		return;
	_isJumpCommand = false;

	if (_isJumping)
		ResetJumpAction();
}
void MoveScript::StopMove(const int& factor) noexcept
{
	auto moveVec = _rigidBody->GetVelocity();
	if (moveVec.x * factor > 0) // Moving same direction
	{
		moveVec.x -= factor * _reduceSpeed;
		moveVec.x = moveVec.x * factor < 0 ? 0 : moveVec.x;
	}
	_rigidBody->SetVelocity(moveVec);
}
#pragma endregion

void MoveScript::Load(nlohmann::json& input)
{
	_jumpStrength	= input[Fields::Player::_jumpStrengthField].get<float>();
	_speedCap		= input[Fields::Player::_speedCapField].get<float>();
	_speedRamUp		= input[Fields::Player::_speedRamUpField].get<float>();
	_reduceSpeed	= input[Fields::Player::_reduceSpeed].get<float>();

	_gsDropFactor	= input[Fields::Player::_gsDropFactor].get<float>();
	_minGravityScale= input[Fields::Player::_gsMin].get<float>();
}

void MoveScript::OnStart()
{
	_rigidBody = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_baseGravityScale = _rigidBody->GetGravityScale();

	_isRunningField = _animator->GetField<bool>(Fields::Player::_isRunning);
	_isGroundedField = _animator->GetField<bool>(Fields::Player::_isGrounded);
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
	CheckDirection(_isMoveLeftCommand, -1, totalVel);
	CheckDirection(_isMoveRightCommand, 1, totalVel);

	totalVel *= _speedRamUp * GameTimer::GetDeltaTime();
	if (_moveVec.x * totalVel < 0.0f)
		_moveVec.x += totalVel;
	else if (abs(_moveVec.x) < _speedCap)
		_moveVec.x += totalVel;

	_isRunningField.lock()->SetValue(IsWalking());
}

void MoveScript::CheckDirection(const bool& keyDown, const int& factor, float& totalVel) noexcept
{
	if (keyDown)
	{
		totalVel = static_cast<float>(factor);
		if (isFlipped != (factor < 0))
		{
			GetGameObject()->GetTransform().SetScale(static_cast<float>(factor), 1, 1);
			isFlipped = (factor < 0);
		}
	}
}

bool MoveScript::IsWalking() const noexcept
{
	return _isMoveLeftCommand ^ _isMoveRightCommand;
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
