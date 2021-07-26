#pragma once
#include "BaseEnemySound.h"

class EnemySound: public BaseEnemySound
{
public:
	void PlayDeadSound();
	void StopDeadSound();

	ScriptBase* Clone() const override;
private:
	REGISTER_START(EnemySound);
};