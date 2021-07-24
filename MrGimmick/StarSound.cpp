#include "StarSound.h"
#include "FieldNames.h"

REGISTER_FINISH(StarSound, BaseSoundScript) {}

void StarSound::OnStart()
{
	_soundManager = GET_FIRST_SCRIPT_OF_TYPE(SoundManager);
}

void StarSound::PlayBounceSound()
{
	_soundManager->Play(Fields::SoundManager::_bounce);
}

void StarSound::StopBounceSound()
{
	_soundManager->Stop(Fields::SoundManager::_bounce);
}

void StarSound::PlayExplodeSound()
{
	_soundManager->Play(Fields::SoundManager::_explode);
}

void StarSound::StopExplodeSound()
{
	_soundManager->Stop(Fields::SoundManager::_explode);
}

ScriptBase* StarSound::Clone() const
{
	auto clone = dynamic_cast<StarSound*>(ScriptBase::Clone());

	return clone;
}
