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
	sound = Audio({ L"clsn4.wav", L"clsn1.wav" }, 0.037f, 0.1f, std::random_device()());
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
	{
		move.y += _jumpStrength;
		sound.PlayAt(1.0f, 1);
	}
	if (left->Check())
	{
		move.x -= 1;
		sound.PauseAt(1);
		if (!isFlipped)
		{
			_ownerObj->SetScale(-1, 1, 1);
			isFlipped = true;
		}
	}
	if (right->Check())
	{
		move.x += 1;
		sound.ContinueAll();
		if (isFlipped)
		{
			_ownerObj->SetScale(1, 1, 1);
			isFlipped = false;
		}
	}
	if (down->Check())
	{
		sound.StopAll();
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