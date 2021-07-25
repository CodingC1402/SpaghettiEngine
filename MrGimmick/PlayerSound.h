#pragma once
#include "BaseSoundScript.h"

CLASS_FORWARD_DECLARATION(PlayerSound);

class PlayerSound : public BaseSoundScript
{
public:
	void OnStart() override;
	void OnEnabled() override;
	void OnDisabled() override;

	void PlayHB();
	void StopHB();
	bool IsPlayingHB();

	void PlayJF();
	void StopJF();
	bool IsPlayingJF();

	void StopAllMusic();

	void PlayJumpSound();
	void StopJumpSound();

	void PlayLandingSound();
	void StopLandingSound();

	void PlayStarCreateSound();
	void StopStarCreateSound();

	void PlayAttackSound();
	void StopAttackSound();

	[[nodiscard]] static PlayerSound* GetCurrentPlayerSound();
protected:
	static inline WPlayerSound __currentInstance;
private:
	REGISTER_START(PlayerSound);
};