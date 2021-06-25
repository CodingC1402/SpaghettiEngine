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

	PScriptBase Clone() const override;
protected:
	SInputAll _leftInput;
	SInputAll _rightInput;
	SInputAll _jumpInput;

	BoolField _isRunningField;
	BoolField _isGroundedField;

	RigidBody2D* _rigidBody;
	Animator* _animator;

	float _speedCap = 150;
	float _jumpStrength = 200;
	float _speedRamUp = 700;

	float _baseGravityScale = 10;
	float _gravityScale = 0;
	float _minGravityScale = 1;
	float _gsDrop = 9; // how much gravity scale drop in 1 sec
	bool _isJumping = false; // Use to check if it's in a jumping phase

	bool _isGrounded = false;

	Vector3 _moveVec;
	bool isFlipped = false;
private:
	REGISTER_START(MoveScript);
};

