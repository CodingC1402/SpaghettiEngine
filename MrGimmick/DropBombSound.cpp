#include "DropBombSound.h"
#include "FieldNames.h"

REGISTER_FINISH(DropBombSound, BaseSoundScript) {}

void DropBombSound::PlayDropSound()
{
	_soundManager->Play(Fields::SoundManager::_drop);
	_soundManager->Play(Fields::SoundManager::_dead);
}

void DropBombSound::StopDropSound()
{
	_soundManager->Stop(Fields::SoundManager::_drop);
	_soundManager->Stop(Fields::SoundManager::_dead);
}

ScriptBase* DropBombSound::Clone() const
{
	auto clone = dynamic_cast<DropBombSound*>(ScriptBase::Clone());

	return clone;
}
