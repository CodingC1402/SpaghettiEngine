#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(const std::initializer_list<std::wstring>& wavFiles, float freqDev, float masterVol, unsigned int seed)
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

void AudioPlayer::PlayAt(float vol, int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Play(freqDist(rng), vol * masterVolume);
}

void AudioPlayer::PlayRandom(float vol)
{
	sounds[soundDist(rng)].Play(freqDist(rng), vol * masterVolume);
}

void AudioPlayer::PlayAll(float vol)
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Play(freqDist(rng), vol * masterVolume);
	}
}

void AudioPlayer::ContinueAt(int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Continue();
}

void AudioPlayer::ContinueAll()
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Continue();
	}
}

void AudioPlayer::PauseAt(int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Pause();
}

void AudioPlayer::PauseAll()
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Pause();
	}
}

void AudioPlayer::StopAt(int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].Stop();
}

void AudioPlayer::StopAll()
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).Stop();
	}
}

void AudioPlayer::ChangeVolumeAt(float vol, int pos)
{
	if (pos >= sounds.size())
		return;

	sounds[pos].ChangeVolume(vol * masterVolume);
}

void AudioPlayer::ChangeVolumeAll(float vol)
{
	for (auto i = sounds.begin(); i != sounds.end(); i++)
	{
		(*i).ChangeVolume(vol * masterVolume);
	}
}

void AudioPlayer::ChangeMasterVolume(float vol)
{
	masterVolume = vol;
}
