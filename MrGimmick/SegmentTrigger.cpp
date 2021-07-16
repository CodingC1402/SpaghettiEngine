#include "SegmentTrigger.h"
#include "FieldNames.h"
#include "LoadingJson.h"
#include "Setting.h"
#include "DebugRenderer.h"
#include "PhysicCollide.h"

REGISTER_FINISH(SegmentTrigger, ScriptBase) {}

void SegmentTrigger::OnFixedUpdate()
{
	std::set<GameObj*> gameObjList;
	PhysicCollide::GetCollidedWithRectangle(GetGameObject(),
		gameObjList,
		_center,
		_width,
		_height,
		Fields::SpecialTag::GetStarOrCharacterTag(),
		PhysicCollide::FilterMode::Collide
	);

	bool triggerSet = false;
	for (auto& obj : gameObjList)
		if (obj->GetTag().Contain(Fields::SpecialTag::GetPlayerTag()))
			triggerSet = true;
		else
			obj->CallDestroy();

	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangle(
			Vector3(_center.x - _width / 2.0f, _center.y + _height / 2.0f, 0), 
			_width, 
			_height, 
			GetWorldMatrix());
	}

	if (triggerSet)
	{
		_playerInside = true;
		if (_shouldTrigger)
		{
			SetState();
			_shouldTrigger = false;
		}
	}
	else
	{
		_playerInside = false;
		_shouldTrigger = true;
	}
}

void SegmentTrigger::Load(nlohmann::json& input)
{
	_segmentA = dynamic_cast<PGameObj>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
	_segmentB = dynamic_cast<PGameObj>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][1][LoadingJson::Field::idField].get<CULL>()));

	_width = input[Fields::SegmentTrigger::_width].get<float>();
	_height = input[Fields::SegmentTrigger::_height].get<float>();
	_center = Vector3(input[Fields::SegmentTrigger::_center]);
}

SegmentTrigger::State SegmentTrigger::GetState() const noexcept
{
	return _state;
}

void SegmentTrigger::SetState() noexcept
{
	if (_state != State::LoadingSegmentA)
	{
		_segmentA->Enable();
		_segmentB->Disable();
		_state = State::LoadingSegmentA;
	}
	else
	{
		_segmentB->Enable();
		_segmentA->Disable();
		_state = State::LoadingSegmentB;
	}
}
