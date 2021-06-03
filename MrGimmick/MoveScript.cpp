#include "MoveScript.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "SMath.h"

REGISTER_FINISH(MoveScript);

void MoveScript::Load(nlohmann::json& input)
{
	_jumpStrength = input[JumpStrengthField].get<float>();
	_speedCap = input[SpeedCapField].get<float>();
	_speedRamUp = input[SpeedRamUpField].get<float>();
}

MoveScript::MoveScript(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(MoveScript);
}

void MoveScript::OnStart()
{
	_rigidBody = std::dynamic_pointer_cast<RigidBody2D>(_ownerObj->GetScript("RigidBody2D")->GetSharedPtr());

	up = InputSystem::GetInput("MoveUp");
	down = InputSystem::GetInput("MoveDown");
	left = InputSystem::GetInput("MoveLeft");
	right = InputSystem::GetInput("MoveRight");
}

void MoveScript::OnUpdate()
{
	move.x = 0;
	move.y = 0;
	move.z = 0;

	if (up->Check())
		move.y += _jumpStrength;
	if (left->Check())
	{
		move.x -= 1;
		if (!isFlipped)
		{
			_ownerObj->SetScale(-1, 1, 1);
			isFlipped = true;
		}
	}
	if (right->Check())
	{
		move.x += 1;
		if (isFlipped)
		{
			_ownerObj->SetScale(1, 1, 1);
			isFlipped = false;
		}
	}

	move.x *= _speedRamUp * GameTimer::GetDeltaTime();

	Vector3 velocity = _rigidBody.lock()->GetVelocity();
	if (move.x * velocity.x < 0)
		velocity.x += move.x;
	else if (abs(velocity.x) < _speedCap)
		velocity.x += move.x;
	velocity.y += move.y;
	_rigidBody.lock()->SetVelocity(velocity);
}