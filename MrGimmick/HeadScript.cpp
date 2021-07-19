#include "HeadScript.h"
#include "FieldNames.h"
#include "PhysicCollide.h"
#include "Setting.h"
#include "DebugRenderer.h"
#include <set>

constexpr unsigned DEBUG_COLOR = 0xFFFF0000;

REGISTER_FINISH(HeadScript, ScriptBase) {}

void HeadScript::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
}

void HeadScript::OnFixedUpdate()
{
	std::set<GameObj*> gameObjList;
	PhysicCollide::GetCollidedWithRectangle(
		GetGameObject(),
		gameObjList,
		_center,
		_width,
		_height,
		Fields::SpecialTag::GetPlatformTag(),
		PhysicCollide::FilterMode::Collide
	);
	_moveScript->SetAllowJump(gameObjList.empty());

	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangleFromCenter(_center, _width, _height, GetWorldMatrix(), DEBUG_COLOR);
	}
}

void HeadScript::Load(nlohmann::json& input)
{
	ScriptBase::Load(input);

	_center = input[Fields::HeadScript::_center];
	_width = input[Fields::HeadScript::_width].get<float>();
	_height = input[Fields::HeadScript::_height].get<float>();
}

PScriptBase HeadScript::Clone() const
{
	auto clone = dynamic_cast<HeadScript*>(ScriptBase::Clone());

	clone->_center = _center;
	clone->_width = _width;
	clone->_height = _height;
	clone->_isCollideWithPlatform = _isCollideWithPlatform;

	return clone;
}
