#include "SegmentTrigger.h"
#include "FieldNames.h"
#include "LoadingJson.h"

REGISTER_FINISH(SegmentTrigger, ScriptBase) {}

void SegmentTrigger::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Contain(Fields::Player::_player))
		_playerInside = true;
}

void SegmentTrigger::OnFixedUpdate()
{
	if (_playerInside)
	{
		if (_shouldTrigger)
		{
			SetState();
			_shouldTrigger = false;
		}
		_playerInside = false;
	}
	else
	{
		_shouldTrigger = true;
	}
}

void SegmentTrigger::Load(nlohmann::json& input)
{
	GameObj* objA = dynamic_cast<PGameObj>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0].get<CULL>()));
	GameObj* objB = dynamic_cast<PGameObj>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][1].get<CULL>()));

	_segmentA = dynamic_cast<LevelSegment*>(objA->GetScriptContainer().GetItemType(TYPE_NAME(LevelSegment)));
	_segmentB = dynamic_cast<LevelSegment*>(objB->GetScriptContainer().GetItemType(TYPE_NAME(LevelSegment)));
}

SegmentTrigger::State SegmentTrigger::GetState() const noexcept
{
	return _state;
}

void SegmentTrigger::SetState() noexcept
{
	if (_state != State::LoadingSegmentA)
	{
		_segmentA->PlayerEnter();
		_segmentB->PlayerExit();
		_state = State::LoadingSegmentA;
		if (_nextTrigger)
			_nextTrigger->Disable();
		if (_previousTrigger)
			_previousTrigger->Enable();
	}
	else
	{
		_segmentB->PlayerEnter();
		_segmentA->PlayerExit();
		_state = State::LoadingSegmentB;
		if (_nextTrigger)
			_nextTrigger->Enable();
		if (_previousTrigger)
			_previousTrigger->Disable();
	}
}
