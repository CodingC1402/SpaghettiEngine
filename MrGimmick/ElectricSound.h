#pragma once
#include "EnemySound.h"

class ElectricSound : public EnemySound
{
public:
	void PlayElectricSound();
	void StopElectricSound();

	void PlayElectricWaveSound();
	void StopElectricWaveSound();

	ScriptBase* Clone() const override;
private:
	REGISTER_START(ElectricSound);
};