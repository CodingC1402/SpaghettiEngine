#include "SoundTriggerBox.h"
#include "DebugRenderer.h"
#include "FieldNames.h"
#include "PhysicCollide.h"
#include "PlayerScript.h"
#include "Setting.h"

REGISTER_FINISH(SoundTriggerBox, ScriptBase) {}
SoundTriggerBox* SoundTriggerBox::__currentScript = nullptr;

constexpr unsigned BoundingBoxColor = 0xFFFFCC00;
constexpr unsigned TriggerZoneColor = 0xFFFF00FA;

void SoundTriggerBox::OnFixedUpdate()
{
	_isPlayerInside = false;
	std::set<GameObj*> objList;
	for (auto& rect : _triggerZones)
	{
		PhysicCollide::GetCollidedWithRectangle(
			GetGameObject(),
			objList,
			rect.GetCenter(),
			rect.GetWidth(),
			rect.GetHeight(),
			Fields::SpecialTag::GetPlayerTag(),
			PhysicCollide::FilterMode::Collide
		);
		if (!objList.empty())
		{
			_isPlayerInside = true;
			if ((__currentScript && !__currentScript->_isPlayerInside) || !__currentScript)
				__currentScript = this;
			break;
		}
	}

	// Debug
	if constexpr (Setting::IsDebugMode())
	{
		for (auto& rect : _triggerZones)
		{
			DebugRenderer::DrawRectangleFromCenter(
				rect.GetCenter(),
				rect.GetWidth(),
				rect.GetHeight(),
				GetWorldMatrix(),
				TriggerZoneColor
			);
		}
	}
}

void SoundTriggerBox::OnDisabled()
{
	if (__currentScript == this)
		__currentScript = nullptr;
}

void SoundTriggerBox::Load(nlohmann::json& input)
{
	Vector3 center;
	float	width;
	float	height;
	for (auto& trigger : input[Fields::CameraBoundingBox::_triggers])
	{
		center = trigger[Fields::CameraBoundingBox::_center];
		width = trigger[Fields::CameraBoundingBox::_width].get<float>();
		height = trigger[Fields::CameraBoundingBox::_height].get<float>();

		_triggerZones.push_back(CRectangle(center, width, height));
	}
}
