#include "BaseEnemySound.h"
#include "FieldNames.h"

REGISTER_FINISH(BaseEnemySound, BaseSoundScript) {}

void BaseEnemySound::OnStart()
{
	_health = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);

	_health->AddToHealthEvent([&](const int& health, const int& delta) {
		this->Play(health, delta);
		});	
	
	BaseSoundScript::OnStart();
}

void BaseEnemySound::PlayDeadSound()
{
	_soundManager->Play(Fields::SoundManager::_dead);
}

void BaseEnemySound::StopDeadSound()
{
	_soundManager->Stop(Fields::SoundManager::_dead);
}

void BaseEnemySound::Play(const int& health, const int& delta)
{
	if (health <= 0)
	{
		PlayDeadSound();
	}
}

ScriptBase* BaseEnemySound::Clone() const
{
	auto clone = dynamic_cast<BaseEnemySound*>(ScriptBase::Clone());

	return clone;
}
