#pragma once
#include "ScriptBase.h"
#include "Camera.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "RigidBody2D.h"
#include "Animator.h"

class MoveScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input);
	void OnStart() override;
	void OnUpdate() override;

#pragma region MoveCommand
	void StartMoveLeft() noexcept;
	void StartMoveRight() noexcept;

	void StopMoveLeft() noexcept;
	void StopMoveRight() noexcept;

	void StartJump() noexcept;
	void StopJump() noexcept;
#pragma endregion

	void SetGrounded(bool value) noexcept; // Manage by PlayerfeetScript
	[[nodiscard]] bool GetGrounded() const noexcept;
	[[nodiscard]] bool IsFlipped() const noexcept;

	void SetAllowJump(bool value) noexcept;
	[[nodiscard]] bool IsAllowJump() const noexcept;
	[[nodiscard]] bool IsWalking() const noexcept;
	void ResetJumpAction();

	PScriptBase Clone() const override;
protected:
	void CheckDirection(const bool& keyDown, const int& factor, float& totalVel) noexcept;

	void JumpAction();
	void MoveAction();

	void StopMove(const int& factor) noexcept;
protected:
#pragma region MoveCommand
	bool _isMoveLeftCommand;
	bool _isMoveRightCommand;
	bool _isJumpCommand;
#pragma endregion

	BoolField _isRunningField;
	BoolField _isGroundedField;

	RigidBody2D* _rigidBody = nullptr;
	Animator* _animator = nullptr;

	float _reduceSpeed = 60; // Reduce the speed when release button
	float _speedCap = 150;
	float _jumpStrength = 50;
	float _speedRamUp = 700;

	float _baseGravityScale = 10;
	float _gravityScale = 0;
	float _minGravityScale = 2;
	float _gsDropFactor = 2; // how much gravity scale drop in 1 sec
	bool _isJumping = false; // Use to check if it's in a jumping phase

	bool _isGrounded = false;
	bool _isAllowJump = true;

	Vector3 _moveVec;
	bool isFlipped = false;
private:
	REGISTER_START(MoveScript);
};

