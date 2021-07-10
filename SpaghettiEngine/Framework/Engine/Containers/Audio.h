#pragma once
#include <memory>
#include <random>
#include <initializer_list>
#include "Sound.h"
#include "ResourceContainer.h"

typedef class Audio* PAudio;
typedef std::shared_ptr<Audio> SAudio;
typedef std::unique_ptr<Audio> UAudio;

class Audio : public Resource
{
	friend class AudioContainer;
public:
	Audio();
	void Load(const std::string& path) override;

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
	bool isLoop;
};

class AudioContainer : public Container<Audio>
{
	friend class SceneManager;
public:
	AudioContainer();
};