#pragma once
#include "BaseSoundScript.h"

CLASS_FORWARD_DECLARATION(PlayerSound);

class PlayerSound : public BaseSoundScript
{
public:
	void OnStart() override;
	void OnEnabled() override;
	void OnDisabled() override;

#pragma region Music
	void PlayHappyBirthday();
	void StopHappyBirthday();
	bool IsPlayingHappyBirthday();

	void PlayJustFriends();
	void StopJustFriends();
	bool IsPlayingJustFriends();

	void PlayAporia();
	void StopAporia();
	bool IsPlayingAporia();

	void StopAllMusic();
#pragma endregion

#pragma region Movement
	void PlayJumpSound();
	void StopJumpSound();

	void PlayLandingSound();
	void StopLandingSound();

	void PlayStarCreateSound();
	void StopStarCreateSound();

	void PlayAttackSound();
	void StopAttackSound();

	void PlayTakeDamageSound();
	void StopTakeDamageSound();

	void PlayDeadSound();
	void StopDeadSound();
#pragma endregion

#pragma region Others
	void PlayLiveUpSound();
	void StopLiveUpSound();

	void PlayTresureSound();
	void StopTresureSound();

	void PlayMaxHealthUpSound();
	void StopMaxHealthUpSound();
#pragma endregion


	[[nodiscard]] static PlayerSound* GetCurrentPlayerSound();
protected:
	static inline WPlayerSound __currentInstance;
private:
	REGISTER_START(PlayerSound);
};