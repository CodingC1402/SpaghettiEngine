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

class Mixer : public Resource
{
	friend class MixerContainer;
public:
	Mixer();
	void Load(const std::string& path) override;

	[[nodiscard]] int GetIndexPosition(std::string name);

	void PlayAt(float vol, int pos);
	void PlayRandom(float vol);
	void PlayAll(float vol);

	void ContinueAt(int pos);
	void ContinueAll();

	void PauseAt(int pos);
	void PauseAll();

	void StopAt(int pos);
	void StopAll();

	void ChangeVolumeAt(float vol, int pos);
	void ChangeVolumeAll(float vol);
	void ChangeMasterVolume(float vol);
protected:
	float masterVolume = 1.0f;
	std::mt19937 rng;
	std::uniform_int_distribution<unsigned int> soundDist;
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