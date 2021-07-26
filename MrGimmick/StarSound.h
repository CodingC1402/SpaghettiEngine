#pragma once
#include "BaseSoundScript.h"

class StarSound : public BaseSoundScript
{
public:
	void PlayBounceSound();
	void StopBounceSound();

	void PlayExplodeSound();
	void StopExplodeSound();
	ScriptBase* Clone() const override;
private:
	REGISTER_START(StarSound);
};