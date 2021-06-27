#pragma once
#include "ScriptBase.h"
#include "Camera.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "RigidBody2D.h"
#include "Animator.h"


typedef std::shared_ptr<InputAll> SInputAll;

class MoveScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input);
	MoveScript(PScene owner);
	void OnStart() override;
	void OnUpdate() override;

	void OnCollideEnter(CollideEvent& e) override;
	void OnCollideExit(CollideEvent& e) override;

	void SetGrounded(bool value) noexcept; // Manage by PlayerfeetScript
	[[nodiscard]] bool GetGrounded() const noexcept;

	void JumpAction();
	void ResetJumpAction();
	void MoveAction();
	void CheckDirection(const bool& keyRelease, const bool& keyDown, const int& factor, bool& directionMove, float& totalVel) noexcept;

	[[nodiscard]] bool IsWalking() const noexcept;

	PScriptBase Clone() const override;
protected:
	SInputAll _leftInput;
	SInputAll _rightInput;
	SInputAll _jumpInput;

	BoolField _isRunningField;
	BoolField _isGroundedField;

	RigidBody2D* _rigidBody;
	Animator* _animator;

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

	bool _isWalkingLeft = false;
	bool _isWalkingRight = false;

	Vector3 _moveVec;
	bool isFlipped = false;
private:
	REGISTER_START(MoveScript);
};

