#include "PlayerSound.h"
#include "FieldNames.h"

REGISTER_FINISH(PlayerSound, BaseSoundScript) {}

void PlayerSound::OnStart()
{
	//__currentInstance = std::dynamic_pointer_cast<PlayerSound>(GetSharedPtr());
	BaseSoundScript::OnStart();
	PlayHappyBirthday();
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

#pragma region HappyBirthday
void PlayerSound::PlayHappyBirthday()
{
	_soundManager->Play(Fields::SoundManager::_happyBirthday);
}

void PlayerSound::StopHappyBirthday()
{
	_soundManager->Stop(Fields::SoundManager::_happyBirthday);
}

bool PlayerSound::IsPlayingHappyBirthday()
{
	return _soundManager->IsPlaying(Fields::SoundManager::_happyBirthday);
}
#pragma endregion

#pragma region JustFriends
void PlayerSound::PlayJustFriends()
{
	_soundManager->Play(Fields::SoundManager::_justFriends);
}

void PlayerSound::StopJustFriends()
{
	_soundManager->Stop(Fields::SoundManager::_justFriends);
}

bool PlayerSound::IsPlayingJustFriends()
{
	return _soundManager->IsPlaying(Fields::SoundManager::_justFriends);
}
#pragma endregion

#pragma region Aporia
void PlayerSound::PlayAporia()
{
	_soundManager->Play(Fields::SoundManager::_aporia);
}

void PlayerSound::StopAporia()
{
	_soundManager->Stop(Fields::SoundManager::_aporia);
}

bool PlayerSound::IsPlayingAporia()
{
	return _soundManager->IsPlaying(Fields::SoundManager::_aporia);
}
#pragma endregion


void PlayerSound::StopAllMusic()
{
	StopHappyBirthday();
	StopJustFriends();
	StopAporia();
}

#pragma region Movement
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

void PlayerSound::PlayTakeDamageSound()
{
	_soundManager->Play(Fields::SoundManager::_takeDamage);
}

void PlayerSound::StopTakeDamageSound()
{
	_soundManager->Stop(Fields::SoundManager::_takeDamage);
}

void PlayerSound::PlayDeadSound()
{
	_soundManager->Play(Fields::SoundManager::_dead);
}

void PlayerSound::StopDeadSound()
{
	_soundManager->Stop(Fields::SoundManager::_dead);
}
#pragma endregion

#pragma region Others
void PlayerSound::PlayLiveUpSound()
{
	_soundManager->Play(Fields::SoundManager::_liveUp);
}

void PlayerSound::StopLiveUpSound()
{
	_soundManager->Stop(Fields::SoundManager::_liveUp);
}

void PlayerSound::PlayTresureSound()
{
	_soundManager->Play(Fields::SoundManager::_tresure);
}

void PlayerSound::StopTresureSound()
{
	_soundManager->Stop(Fields::SoundManager::_tresure);
}

void PlayerSound::PlayMaxHealthUpSound()
{
	_soundManager->Play(Fields::SoundManager::_maxHealthUp);
}

void PlayerSound::StopMaxHealthUpSound()
{
	_soundManager->Stop(Fields::SoundManager::_maxHealthUp);
}
#pragma endregion


PlayerSound* PlayerSound::GetCurrentPlayerSound()
{
	return __currentInstance.expired() ? nullptr : __currentInstance.lock().get();
}
