#pragma once
#include "BaseSoundScript.h"
#include "HealthScript.h"

class BaseEnemySound : public BaseSoundScript
{
public:
	void OnStart() override;
	void OnDisabled() override;

	void PlayDeadSound();
	void StopDeadSound();

	ScriptBase* Clone() const override;
protected:
	HealthScript* _health = nullptr;
private:
	REGISTER_START(BaseEnemySound);
};