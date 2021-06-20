#pragma once
#include "Sound.h"
#include <random>
#include <initializer_list>

class Audio
{
public:
	Audio() = default;
	Audio(const std::initializer_list<std::wstring>& wavFiles,
		float freqDev, unsigned int seed)
		:
		rng(seed),
		freqDist(1.0f, freqDev),
		soundDist(0, wavFiles.size() - 1)
	{
		for (auto& f : wavFiles)
		{
			sounds.emplace_back(f);
		}
	}
	void PlayAt(float vol, int pos)
	{
		sounds[pos].Play(freqDist(rng), vol);
	}
	void PlayRandom(float vol)
	{
		sounds[soundDist(rng)].Play(freqDist(rng), vol);
	}
	void PlayAll(float vol)
	{
		for (auto i = sounds.begin(); i != sounds.end(); i++)
		{
			(*i).Play(freqDist(rng), vol);
		}
	}
	void ContinueAll()
	{
		for (auto i = sounds.begin(); i != sounds.end(); i++)
		{
			(*i).Continue();
		}
	}
	void PauseAll()
	{
		for (auto i = sounds.begin(); i != sounds.end(); i++)
		{
			(*i).Pause();
		}
	}
	void StopAll ()
	{
		for (auto i = sounds.begin(); i != sounds.end(); i++)
		{
			(*i).Stop();
		}
	}
	void ChangeVolume(float vol)
	{
		for (auto i = sounds.begin(); i != sounds.end(); i++)
		{
			(*i).ChangeVolume(vol);
		}
	}
private:
	float masterVol = 100;
	std::mt19937 rng;
	std::uniform_int_distribution<unsigned int> soundDist;
	std::normal_distribution<float> freqDist;
	std::vector<Sound> sounds;
};