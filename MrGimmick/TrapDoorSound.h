#pragma once
#include "BaseSoundScript.h"

class TrapDoorSound : public BaseSoundScript
{
public:
	void PlayTickSound();
	void StopTickSound();

	void PlayOpenTrapSound();
	void StopOpenTrapSound();
	ScriptBase* Clone() const override;
private:
	REGISTER_START(TrapDoorSound);
};