#include "EnemySound.h"
#include "FieldNames.h"

REGISTER_FINISH(EnemySound, BaseEnemySound) {}

void EnemySound::PlayDeadSound()
{
	_soundManager->Play(Fields::SoundManager::_dead);
}

void EnemySound::StopDeadSound()
{
	_soundManager->Stop(Fields::SoundManager::_dead);
}

ScriptBase* EnemySound::Clone() const
{
	auto clone = dynamic_cast<EnemySound*>(ScriptBase::Clone());

	return clone;
}
