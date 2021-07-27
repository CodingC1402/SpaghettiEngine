#pragma once
#include "BaseSoundScript.h"
#include "HealthScript.h"

class BaseEnemySound : public BaseSoundScript
{
public:
	virtual void OnStart() override;

	virtual void PlayDeadSound();
	virtual void StopDeadSound();

	virtual void Play(const int& health, const int& delta);

	ScriptBase* Clone() const override;
protected:
	HealthScript* _health = nullptr;
private:
	REGISTER_START(BaseEnemySound);
};