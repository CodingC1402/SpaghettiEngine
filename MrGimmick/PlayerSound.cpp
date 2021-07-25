#include "PlayerSound.h"
#include "FieldNames.h"

REGISTER_FINISH(PlayerSound, BaseSoundScript) {}

void PlayerSound::OnStart()
{
	//__currentInstance = std::dynamic_pointer_cast<PlayerSound>(GetSharedPtr());
	BaseSoundScript::OnStart();
	PlayHB();
}

void PlayerSound::OnEnabled()
{
	__currentInstance = std::dynamic_pointer_cast<PlayerSound>(GetSharedPtr());
}

void PlayerSound::OnDisabled()
{
	__currentInstance.reset();
	StopAllMusic();
	StopJumpSound();
	StopLandingSound();
	StopStarCreateSound();
	StopAttackSound();
}

void PlayerSound::PlayHB()
{
	_soundManager->Play(Fields::SoundManager::_happyBirthday);
}

void PlayerSound::StopHB()
{
	_soundManager->Stop(Fields::SoundManager::_happyBirthday);
}

bool PlayerSound::IsPlayingHB()
{
	return _soundManager->IsPlaying(Fields::SoundManager::_happyBirthday);
}

void PlayerSound::PlayJF()
{
	_soundManager->Play(Fields::SoundManager::_justFriends);
}

void PlayerSound::StopJF()
{
	_soundManager->Stop(Fields::SoundManager::_justFriends);
}

bool PlayerSound::IsPlayingJF()
{
	return _soundManager->IsPlaying(Fields::SoundManager::_justFriends);
}

void PlayerSound::StopAllMusic()
{
	StopHB();
	StopJF();
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
