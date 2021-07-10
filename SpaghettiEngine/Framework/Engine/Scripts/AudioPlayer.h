#pragma once
#include "Audio.h"
#include "ScriptBase.h"

class AudioPlayer : public ScriptBase
{
public:
	virtual void Load(nlohmann::json& inputObject) override;
	virtual void Play();

	PScriptBase Clone() const override;
protected:
	SAudio _audio;
private:
	REGISTER_START(AudioPlayer);
};