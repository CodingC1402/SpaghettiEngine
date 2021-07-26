#pragma once
#include "BaseSoundScript.h"

class AttackSound : public BaseSoundScript
{
public:
	void PlayBounceSound();
	void StopBounceSound();

	void PlayExplodeSound();
	void StopExplodeSound();

	void PlayAttackSound();
	void StopAttackSound();

	ScriptBase* Clone() const override;
private:
	REGISTER_START(AttackSound);
};