#include "CameraBoundingBox.h"
#include "DebugRenderer.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "PhysicCollide.h"
#include "PlayerScript.h"
#include "Setting.h"


REGISTER_FINISH(CameraBoundingBox, ScriptBase) {}
CameraBoundingBox* CameraBoundingBox::__currentScript = nullptr;
bool CameraBoundingBox::__tempDisable = false;

constexpr unsigned BoundingBoxColor = 0xFFFFCC00;
constexpr unsigned TriggerZoneColor = 0xFFFF00FA;

void CameraBoundingBox::OnFixedUpdate()
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

		float boundingBoxWidth = _maxX - _minX;
		float boundingBoxHeight = _maxY - _minY;
		Vector3 boundingBoxCenter;
		boundingBoxCenter.x = (_maxX + _minX) / 2.0f;
		boundingBoxCenter.y = (_maxY + _minY) / 2.0f;
		DebugRenderer::DrawRectangleFromCenter(boundingBoxCenter, boundingBoxWidth, boundingBoxHeight, GetWorldMatrix(), BoundingBoxColor);
	}
}

void CameraBoundingBox::OnLateUpdate()
{
	if (__currentScript != this || __tempDisable)
		return;

	auto cameraScript = Graphics::GetActiveCamera();
	cameraScript->RemoveFollow();

	Vector3 playerPos = PlayerScript::GetCurrentPlayer()->GetTransform().GetWorldTransform();
	Vector3 camPos = playerPos;

	float screenWidth = Setting::GetResolution().width / 2.0f;
	float screenHeight = Setting::GetResolution().height / 2.0f;
	camPos.x -= camPos.x + screenWidth > _maxX ? (camPos.x + screenWidth) - _maxX : 0;
	camPos.x += camPos.x - screenWidth < _minX ? _minX - (camPos.x - screenWidth) : 0;
	camPos.y -= camPos.y + screenHeight > _maxY ? (camPos.y + screenHeight) - _maxY : 0;
	camPos.y += camPos.y - screenHeight < _minY ? _minY - (camPos.y - screenHeight) : 0;

	cameraScript->GetGameObject()->GetTransform().SetWorldTransform(camPos);
}

void CameraBoundingBox::OnDisabled()
{
	if (__currentScript == this)
		__currentScript = nullptr;
}

void CameraBoundingBox::Load(nlohmann::json& input)
{
	Vector3 center;
	float	width;
	float	height;
	for (auto& trigger : input[Fields::CameraBoundingBox::_triggers])
	{
		center	= trigger[Fields::CameraBoundingBox::_center];
		width	= trigger[Fields::CameraBoundingBox::_width].get<float>();
		height	= trigger[Fields::CameraBoundingBox::_height].get<float>();

		_triggerZones.push_back(Rectangle(center, width, height));
	}

	_minX = input[Fields::CameraBoundingBox::_minX].get<float>();
	_maxX = input[Fields::CameraBoundingBox::_maxX].get<float>();
	_minY = input[Fields::CameraBoundingBox::_minY].get<float>();
	_maxY = input[Fields::CameraBoundingBox::_maxY].get<float>();
}

void CameraBoundingBox::TempShutDown()
{
	__tempDisable = true;
	Graphics::GetActiveCamera()->SetFollow(PlayerScript::GetCurrentPlayer());
}

void CameraBoundingBox::TurnOn()
{
	__tempDisable = false;
}
