#pragma once
#include "BaseEnemySound.h"

class EnemySound: public BaseEnemySound
{
public:
	virtual void PlayDeadSound();
	virtual void StopDeadSound();

	ScriptBase* Clone() const override;
private:
	REGISTER_START(EnemySound);
};