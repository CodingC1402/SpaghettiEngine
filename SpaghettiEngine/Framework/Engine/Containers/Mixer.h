#pragma once
#include <memory>
#include <random>
#include <initializer_list>
#include <unordered_map>
#include "Sound.h"
#include "ResourceContainer.h"

typedef class Mixer* PMixer;
typedef std::shared_ptr<Mixer> SMixer;
typedef std::unique_ptr<Mixer> UMixer;

// A mixer have multiple sounds

class Mixer : public Resource
{
	friend class MixerContainer;
public:
	Mixer();
	void Load(const std::string& path) override;

	[[nodiscard]] int GetIndexPosition(std::string name);

	// return a pointer channel if in need
	PChannel& PlaySoundAt(float vol, int pos);
	std::vector<PChannel> PlayAllSounds(float vol);

	void ResumeChannelInSoundAt(int pos, PChannel& chan);
	void ResumeSoundAt(int pos);
	void ResumeAllSounds();

	void PauseChannelInSoundAt(int pos, PChannel& chan);
	void PauseSoundAt(int pos);
	void PauseAllSounds();

	void StopChannelInSoundAt(int pos, PChannel& chan);
	void StopSoundAt(int pos);
	void StopAllSounds();

	void ChangeVolumeChannelInSoundAt(float vol, int pos, PChannel& chan);
	void ChangeVolumeSoundAt(float vol, int pos);
	void ChangeVolumeAllSounds(float vol);

	bool IsChannelPlayingInSoundAt(int pos, PChannel chan);
	bool IsSoundPlayingAt(int pos);
	bool IsAllSoundsPlaying();

	void ChangeMasterVolume(float vol);
	[[nodiscard]] float GetMasterVolume();
protected:
	float masterVolume = 1.0f;
	std::mt19937 rng;
	std::normal_distribution<float> freqDist;
	std::vector<Sound> _sounds;
	std::unordered_map<std::string, int> _soundMap;
	bool isLoop;
};

class MixerContainer : public Container<Mixer>
{
	friend class SceneManager;
public:
	MixerContainer();
};