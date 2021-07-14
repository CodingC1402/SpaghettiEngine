#pragma once
#include "Mixer.h"
#include "ScriptBase.h"
#include "LoadingJson.h"

enum PlayMode
{
	OVERLAP,
	NOTHING
};

NLOHMANN_JSON_SERIALIZE_ENUM(PlayMode, {
	{OVERLAP, "Overlap"},
	{NOTHING, "Nothing"}
	})

class SoundSource : public ScriptBase
{
public:
	virtual void Load(nlohmann::json& inputObject) override;
	virtual void OnUpdate() override;

	virtual void Play();
	virtual void Stop();
	virtual void Pause();
	virtual void Resume();

	[[nodiscard]] bool IsRepeat();
	void SetRepeat(bool r);

	[[nodiscard]] float GetDelay();
	void SetDelay(float d);

	[[nodiscard]] float GetVolume();
	void SetVolume(float v);

	void SetMode(PlayMode m);

	PScriptBase Clone() const override;
protected:
	PlayMode mode;
	SMixer _audio;
	PChannel _channel;
	int index;
	bool isRepeat = false;
	bool isStop = true;
	float delay = 0;
	float volume = 1.0f;
	float currentTime = 0;
private:
	REGISTER_START(SoundSource);
};