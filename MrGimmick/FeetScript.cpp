#include "FeetScript.h"
#include "FieldNames.h"
#include "Setting.h"
#include "DebugRenderer.h"
#include "PhysicCollide.h"

constexpr unsigned DEBUG_COLOR = 0xFFFF0000;

REGISTER_FINISH(FeetScript, ScriptBase) {}

void FeetScript::OnStart()
{
	_moveScript = dynamic_cast<MoveScript*>(GetGameObject()->GetScriptContainer().GetItemType("MoveScript"));
}

void FeetScript::OnFixedUpdate()
{
	std::set<GameObj*> collidedGameObject;
	PhysicCollide::GetCollidedWithRectangle(
		GetGameObject(),
		collidedGameObject,
		_position, 
		_width,
		_height,
		Fields::SpecialTag::GetPlatformTag(),
		PhysicCollide::FilterMode::Collide
	);

	if (!collidedGameObject.empty())
		_moveScript->SetGrounded(true);
	else
		_moveScript->SetGrounded(false);


	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangle(
			Vector3(_position.x - _width / 2.0f, _position.y + _height / 2.0f, 0.0f), 
			_width, 
			_height, 
			GetWorldMatrix(),
			DEBUG_COLOR
		);
	}
}

void FeetScript::Load(nlohmann::json& input)
{
	_width = input[Fields::FeetScript::_width].get<float>();
	_height = input[Fields::FeetScript::_height].get<float>();
	_position = input[Fields::FeetScript::_position];
}

PScriptBase FeetScript::Clone() const
{
	auto clone = dynamic_cast<FeetScript*>(ScriptBase::Clone());

	clone->_isGrounded = _isGrounded;
	clone->_isLastGrounded = _isLastGrounded;

	return PScriptBase();
}
