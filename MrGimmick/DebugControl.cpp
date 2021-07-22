// This have to goes first or else the fucking gdi will strike again
#include "CameraBoundingBox.h"
#include "DebugControl.h"
#include "InputSystem.h"
#include "FieldNames.h"
#include "Polygon2DCollider.h"
#include "Physic.h"

REGISTER_FINISH(DebugControl, ScriptBase) {}

void DebugControl::OnStart()
{
	_moveLeft	= std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveLeft));
	_moveRight	= std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveRight));
	_moveUp		= std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveUp));
	_moveDown	= std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveDown));
	_enterDebug	= std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_enterDebug));

	_playerControlScript = GET_FIRST_SCRIPT_OF_TYPE(PlayerControl);
	_rbScript = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
}

void DebugControl::OnUpdate()
{
	if (_enterDebug->CheckKeyPress())
	{
		_isInDebugMode = !_isInDebugMode;
		if (_isInDebugMode)
		{
			_playerControlScript->Disable();
			_rbScript->Disable();
			GetGameObject()->GetScriptContainer().IteratingWithLamda([](PScriptBase script) {
				if (script->GetType() == TYPE_NAME(Polygon2DCollider))
					script->Disable();
			});
			CameraBoundingBox::TempShutDown();
		}
		else
		{
			_playerControlScript->Enable();
			_rbScript->Enable();
			GetGameObject()->GetScriptContainer().IteratingWithLamda([](PScriptBase script) {
				if (script->GetType() == TYPE_NAME(Polygon2DCollider))
					script->Enable();
			});
			CameraBoundingBox::TurnOn();
		}
	}
}

void DebugControl::OnFixedUpdate()
{
	if (_isInDebugMode)
	{
		float delta = _debugFlySpeed * Physic::GetStep();

		Vector3 pos = GetGameObject()->GetTransform().GetTransform();
		if (_moveUp->CheckKeyDown())
			pos.y += delta;
		if (_moveDown->CheckKeyDown())
			pos.y -= delta;

		if (_moveRight->CheckKeyDown())
			pos.x += delta;
		if (_moveLeft->CheckKeyDown())
			pos.x -= delta;
		GetGameObject()->GetTransform().SetTransform(pos);
	}
}

void DebugControl::Load(nlohmann::json& input)
{
	_debugFlySpeed = input[Fields::DebugControl::_debugFlySpeed].get<float>();
}
