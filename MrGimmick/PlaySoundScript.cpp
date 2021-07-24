#include "PlaySoundScript.h"
#include "FieldNames.h"

REGISTER_FINISH(PlaySoundScript, ScriptBase) {}

void PlaySoundScript::Load(nlohmann::json& input)
{

}

void PlaySoundScript::OnStart()
{
	_audio = GET_FIRST_SCRIPT_OF_TYPE(SoundManager);
	
	_moveLeftInput = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveLeft));
	_moveRightInput = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveRight));
	_jumpInput = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_jump));
	_attackInput = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_attack));
}

void PlaySoundScript::OnUpdate()
{
	
}
