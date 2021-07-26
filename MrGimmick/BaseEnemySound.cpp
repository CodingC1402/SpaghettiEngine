#include "BaseEnemySound.h"
#include "FieldNames.h"

REGISTER_FINISH(BaseEnemySound, BaseSoundScript) {}

void BaseEnemySound::OnStart()
{
	_health = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
	BaseSoundScript::OnStart();
}

void BaseEnemySound::OnDisabled()
{
	if (!_health)
		return;

	if (_health->GetHealth() == 0)
		PlayDeadSound();
}

void BaseEnemySound::PlayDeadSound()
{
	_soundManager->Play(Fields::SoundManager::_dead);
}

void BaseEnemySound::StopDeadSound()
{
	_soundManager->Stop(Fields::SoundManager::_dead);
}

ScriptBase* BaseEnemySound::Clone() const
{
	auto clone = dynamic_cast<BaseEnemySound*>(ScriptBase::Clone());

	return clone;
}
