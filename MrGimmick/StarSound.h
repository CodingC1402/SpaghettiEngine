#pragma once
#include "BaseSoundScript.h"

CLASS_FORWARD_DECLARATION(StarSound);

class StarSound : public BaseSoundScript
{
public:
	void OnStart() override;

	void PlayBounceSound();
	void StopBounceSound();

	void PlayExplodeSound();
	void StopExplodeSound();
	ScriptBase* Clone() const override;
protected:
	SoundManager* _soundManager = nullptr;
private:
	REGISTER_START(StarSound);
};