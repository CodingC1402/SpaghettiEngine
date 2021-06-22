#include "Audio.h"

Audio::Audio(const std::initializer_list<std::wstring>& wavFiles, float freqDev, float masterVol, unsigned int seed)
	:
	masterVolume(masterVol),
	rng(seed),
	freqDist(1.0f, freqDev),
	soundDist(0, wavFiles.size() - 1)
{
	for (auto& f : wavFiles)
	{
		sounds.emplace_back(f);
	}
}

void Audio::PlayAt(float vol, int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Play(freqDist(rng), vol * masterVolume);
}

void Audio::PlayRandom(float vol)
{
	sounds[soundDist(rng)].Play(freqDist(rng), vol * masterVolume);
}

void Audio::PlayAll(float vol)
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Play(freqDist(rng), vol * masterVolume);
	}
}

void Audio::ContinueAt(int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Continue();
}

void Audio::ContinueAll()
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Continue();
	}
}

void Audio::PauseAt(int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Pause();
}

void Audio::PauseAll()
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Pause();
	}
}

void Audio::StopAt(int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Stop();
}

void Audio::StopAll()
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Stop();
	}
}

void Audio::ChangeVolumeAt(float vol, int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].ChangeVolume(vol * masterVolume);
}

void Audio::ChangeVolumeAll(float vol)
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).ChangeVolume(vol * masterVolume);
	}
}

void Audio::ChangeMasterVolume(float vol)
{
	masterVolume = vol;
}
