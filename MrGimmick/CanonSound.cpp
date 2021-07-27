#include "CanonSound.h"
#include "FieldNames.h"

REGISTER_FINISH(CanonSound, BaseEnemySound) {}

void CanonSound::PlayShootSound()
{
	_soundManager->Play(Fields::SoundManager::_shoot);
}

void CanonSound::StopShootSound()
{
	_soundManager->Stop(Fields::SoundManager::_shoot);
}

ScriptBase* CanonSound::Clone() const
{
	auto clone = dynamic_cast<CanonSound*>(ScriptBase::Clone());

	return clone;
}
