#include "PlayerSound.h"
#include "FieldNames.h"

REGISTER_FINISH(PlayerSound, BaseSoundScript) {}

void PlayerSound::OnStart()
{
	//__currentInstance = std::dynamic_pointer_cast<PlayerSound>(GetSharedPtr());
	BaseSoundScript::OnStart();
	PlayMusic();
}

void PlayerSound::OnEnabled()
{
	__currentInstance = std::dynamic_pointer_cast<PlayerSound>(GetSharedPtr());
}

void PlayerSound::OnDisabled()
{
	__currentInstance.reset();
	StopMusic();
	StopJumpSound();
	StopLandingSound();
	StopStarCreateSound();
	StopAttackSound();
}

void PlayerSound::PlayMusic()
{
	_soundManager->Play(Fields::SoundManager::_music);
}

void PlayerSound::StopMusic()
{
	_soundManager->Stop(Fields::SoundManager::_music);
}

void PlayerSound::PlayJumpSound()
{
	_soundManager->Play(Fields::SoundManager::_jump);
}

void PlayerSound::StopJumpSound()
{
	_soundManager->Stop(Fields::SoundManager::_jump);
}

void PlayerSound::PlayLandingSound()
{
	_soundManager->Play(Fields::SoundManager::_landing);
}

void PlayerSound::StopLandingSound()
{
	_soundManager->Stop(Fields::SoundManager::_landing);
}

void PlayerSound::PlayStarCreateSound()
{
	_soundManager->Play(Fields::SoundManager::_starCreate);
}

void PlayerSound::StopStarCreateSound()
{
	_soundManager->Stop(Fields::SoundManager::_starCreate);
}

void PlayerSound::PlayAttackSound()
{
	_soundManager->Play(Fields::SoundManager::_attack);
}

void PlayerSound::StopAttackSound()
{
	_soundManager->Stop(Fields::SoundManager::_attack);
}

PlayerSound* PlayerSound::GetCurrentPlayerSound()
{
	return __currentInstance.expired() ? nullptr : __currentInstance.lock().get();
}
