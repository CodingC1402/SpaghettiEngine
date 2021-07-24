#include "BaseSoundScript.h"
#include "FieldNames.h"

REGISTER_FINISH(BaseSoundScript, ScriptBase) {}

void BaseSoundScript::OnStart()
{
	_soundManager = GET_FIRST_SCRIPT_OF_TYPE(SoundManager);
}

void BaseSoundScript::PlayDeadSound()
{
	_soundManager->Play(Fields::SoundManager::_dead);
}

void BaseSoundScript::StopDeadSound()
{
	_soundManager->Stop(Fields::SoundManager::_dead);
}

PScriptBase BaseSoundScript::Clone() const
{
	auto clone = dynamic_cast<BaseSoundScript*>(ScriptBase::Clone());

	return clone;
}
