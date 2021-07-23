#pragma once
#include "ScriptBase.h"
#include "SoundSource.h"

class SoundManager : public ScriptBase
{
public:
	void OnStart() override;
	void OnDisabled() override;
	void Load(nlohmann::json& inputObject) override;

	void Disable(const std::string& name);
	void Enable(const std::string& name);

	void Play(const std::string &name);
	void Stop(const std::string& name);
	void Pause(const std::string& name);
	void Resume(const std::string& name);

	[[nodiscard]] bool IsRepeat(const std::string& name);
	void SetRepeat(bool r, const std::string& name);

	[[nodiscard]] float GetDelay(const std::string& name);
	void SetDelay(float d, const std::string& name);

	[[nodiscard]] float GetVolume(const std::string& name);
	void SetVolume(float v, const std::string& name);

	[[nodiscard]] SoundSource::PlayMode GetMode(const std::string& name);
	void SetMode(SoundSource::PlayMode m, const std::string& name);

	SoundSource* FindInMapByName(const std::string& name);

	PScriptBase Clone() const override;
protected:
	std::unordered_map<std::string, SoundSource*> _soundSources;
private:
	REGISTER_START(SoundManager);
};