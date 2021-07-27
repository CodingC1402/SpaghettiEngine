#pragma once
#include "BaseSoundScript.h"

class DropBombSound : public BaseSoundScript
{
public:
	void PlayDropSound();
	void StopDropSound();

	ScriptBase* Clone() const override;
private:
	REGISTER_START(DropBombSound);
};