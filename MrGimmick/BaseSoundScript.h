#pragma once
#include "SoundManager.h"
#include "ScriptBase.h"

class BaseSoundScript : public ScriptBase
{
public:
	void OnStart() override;

	virtual void PlayDeadSound();
	virtual void StopDeadSound();

	PScriptBase Clone() const override;
protected:
	SoundManager* _soundManager = nullptr;
private:
	REGISTER_START(BaseSoundScript);
};