#include "PlaySoundScript.h"
#include "FieldNames.h"

REGISTER_FINISH(PlaySoundScript, ScriptBase) {}

void PlaySoundScript::Load(nlohmann::json& input)
{

}

void PlaySoundScript::OnStart()
{
	_audio = GET_FIRST_SCRIPT_OF_TYPE(SoundSource);
	
	_play1Input = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveLeft));
	_play2Input = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveRight));
	_stop1Input = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_jump));
	_stop2Input = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_attack));
}

void PlaySoundScript::OnUpdate()
{
	switch (soundLayer)
	{
	case 0:
		if (_play1Input->CheckKeyDown())
		{
			//_audio->Play();
		}
		else if (_stop1Input->CheckKeyDown())
		{
			//_audio->Stop();
		}
		break;
	case 1:
		if (_play1Input->CheckKeyDown())
		{
			//_audio->Play();
		}
		else if (_stop2Input->CheckKeyDown())
		{
			//_audio->Stop();
		}
		break;
	default:
		break;
	}
}
