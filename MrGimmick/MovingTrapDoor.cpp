#include "MovingTrapDoor.h"
#include "PhysicCollide.h"
#include "DebugRenderer.h"
#include "FieldNames.h"
#include "Setting.h"
#include "Polygon2DCollider.h"
#include "GameTimer.h"

REGISTER_FINISH(MovingTrapDoor, ScriptBase) {}
constexpr unsigned DEBUG_COLOR = 0xFFFF0000;

void MovingTrapDoor::OnStart()
{
	_sound = GET_FIRST_SCRIPT_OF_TYPE(TrapDoorSound);
	auto animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_dispatchedField = animator->GetField<bool>("Dispatched");
	_polygonCollider = GET_FIRST_SCRIPT_OF_TYPE(Polygon2DCollider);
	_movableScript = GET_FIRST_SCRIPT_OF_TYPE(MovableScript);
	_movableScript->AddArrivedEvent(
		[&](const Vector3& des) {
			Arrived();
		}
	);
	_delay = _movableScript->GetDelay();
}

void MovingTrapDoor::OnFixedUpdate()
{
	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangleFromCenter(
			_triggerZone.GetCenter(),
			_triggerZone.GetWidth(),
			_triggerZone.GetHeight(),
			GetWorldMatrix(),
			DEBUG_COLOR
		);
	}

	if (_delayCD > 0)
	{
		_delayCD -= GameTimer::GetDeltaTime();
		if (_delayCD <= 0)
		{
			_polygonCollider->Enable();
		}
	}

	if (!_dispatched)
	{
		std::set<GameObj*> objList;
		PhysicCollide::GetCollidedWithRectangle(
			GetGameObject(),
			objList,
			_triggerZone.GetCenter(),
			_triggerZone.GetWidth(),
			_triggerZone.GetHeight(),
			Fields::SpecialTag::GetPlayerTag(),
			PhysicCollide::FilterMode::Collide
		);
		if (!objList.empty())
		{
			_isStartTick = true;
			_movableScript->ForceDispatch();
			_dispatched = 2;
		}
	}

	if (_isStartTick)
		_tick += GameTimer::GetDeltaTime();

	if (_tick > 0.15)
	{
		_sound->PlayTickSound();
		_tick = 0;
	}
}

void MovingTrapDoor::Load(nlohmann::json& input)
{
	_triggerZone = input;
}

ScriptBase* MovingTrapDoor::Clone() const
{
	auto clone = dynamic_cast<MovingTrapDoor*>(ScriptBase::Clone());

	clone->_triggerZone = _triggerZone;

	return clone;
}

void MovingTrapDoor::Arrived()
{
	if (_dispatched == 2)
	{
		_isStartTick = false;
		_sound->StopTickSound();
		_sound->PlayOpenTrapSound();
		_dispatchedField.lock()->SetValue(true);
		_movableScript->Dispatch();
		_polygonCollider->Disable();
		_delayCD = _delay;
	}
	_dispatched--;
}