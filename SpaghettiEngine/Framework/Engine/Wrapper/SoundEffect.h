#pragma once
#include "Sound.h"
#include <random>
#include <initializer_list>

class SoundEffect
{
public:
	SoundEffect() = default;
	SoundEffect(const std::initializer_list<std::wstring>& wavFiles,
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
	void Play(float vol)
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
	void Stop()
	{
		for (auto i = sounds.begin(); i != sounds.end(); i++)
		{
			(*i).Stop();
		}
	}
private:
	std::mt19937 rng;
	std::uniform_int_distribution<unsigned int> soundDist;
	std::normal_distribution<float> freqDist;
	std::vector<Sound> sounds;
};