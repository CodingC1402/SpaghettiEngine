#include "AttackSound.h"
#include "FieldNames.h"

REGISTER_FINISH(AttackSound, BaseSoundScript) {}

void AttackSound::PlayBounceSound()
{
	_soundManager->Play(Fields::SoundManager::_bounce);
	_soundManager->Play(Fields::SoundManager::_bounceExtra);
}

void AttackSound::StopBounceSound()
{
	_soundManager->Stop(Fields::SoundManager::_bounce);
	_soundManager->Stop(Fields::SoundManager::_bounceExtra);
}

void AttackSound::PlayExplodeSound()
{
	_soundManager->Play(Fields::SoundManager::_explode);
}

void AttackSound::StopExplodeSound()
{
	_soundManager->Stop(Fields::SoundManager::_explode);
}

void AttackSound::PlayAttackSound()
{
	_soundManager->Play(Fields::SoundManager::_attack);
}

void AttackSound::StopAttackSound()
{
	_soundManager->Stop(Fields::SoundManager::_attack);
}

ScriptBase* AttackSound::Clone() const
{
	auto clone = dynamic_cast<AttackSound*>(ScriptBase::Clone());

	return clone;
}
