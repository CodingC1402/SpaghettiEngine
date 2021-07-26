#include "TrapDoorSound.h"
#include "FieldNames.h"

REGISTER_FINISH(TrapDoorSound, BaseSoundScript) {}

void TrapDoorSound::PlayTickSound()
{
	_soundManager->Play(Fields::SoundManager::_tick);
}

void TrapDoorSound::StopTickSound()
{
	_soundManager->Stop(Fields::SoundManager::_tick);
}

void TrapDoorSound::PlayOpenTrapSound()
{
	_soundManager->Play(Fields::SoundManager::_openTrap);
}

void TrapDoorSound::StopOpenTrapSound()
{
	_soundManager->Stop(Fields::SoundManager::_openTrap);
}

ScriptBase* TrapDoorSound::Clone() const
{
	auto clone = dynamic_cast<TrapDoorSound*>(ScriptBase::Clone());

	return clone;
}