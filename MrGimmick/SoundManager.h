#pragma once
#include "ScriptBase.h"
#include "SoundSource.h"

class SoundManager : public ScriptBase
{
public:
	void Load(nlohmann::json& inputObject) override;
	void OnUpdate() override;

	void Play();
	void Stop();
	void Pause();
	void Resume();

	[[nodiscard]] bool IsRepeat();
	void SetRepeat(bool r);

	[[nodiscard]] float GetDelay();
	void SetDelay(float d);

	[[nodiscard]] float GetVolume();
	void SetVolume(float v);

	void SetMode(SoundSource::PlayMode m);

	PScriptBase Clone() const override;
protected:
	std::unordered_map<std::string, SoundSource*> a;
private:
	REGISTER_START(SoundManager);
};