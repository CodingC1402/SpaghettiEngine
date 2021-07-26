#include "ElectricSound.h"
#include "FieldNames.h"

REGISTER_FINISH(ElectricSound, EnemySound) {}

void ElectricSound::PlayElectricSound()
{
	_soundManager->Play(Fields::SoundManager::_electric);
}

void ElectricSound::StopElectricSound()
{
	_soundManager->Stop(Fields::SoundManager::_electric);
}

void ElectricSound::PlayElectricWaveSound()
{
	_soundManager->Play(Fields::SoundManager::_electricWave);
}

void ElectricSound::StopElectricWaveSound()
{
	_soundManager->Stop(Fields::SoundManager::_electricWave);
}

ScriptBase* ElectricSound::Clone() const
{
	auto clone = dynamic_cast<ElectricSound*>(ScriptBase::Clone());

	return clone;
}
