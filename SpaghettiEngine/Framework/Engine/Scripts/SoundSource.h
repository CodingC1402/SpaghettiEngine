#pragma once
#include "Mixer.h"
#include "ScriptBase.h"

class SoundSource : public ScriptBase
{
public:
	virtual void Load(nlohmann::json& inputObject) override;
	virtual void Play();

	PScriptBase Clone() const override;
protected:
	SMixer _audio;
	int index;
private:
	REGISTER_START(SoundSource);
};