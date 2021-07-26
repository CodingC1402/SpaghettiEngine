#pragma once
#include "BaseEnemySound.h"

class CanonSound : public BaseEnemySound
{
public:
	void PlayShootSound();
	void StopShootSound();

	ScriptBase* Clone() const override;
private:
	REGISTER_START(CanonSound);
};