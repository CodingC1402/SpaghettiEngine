#include "Mixer.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include "StringConverter.h"
#include "ScriptBase.h"
#include <fstream>

CONTAINER_REGISTER(MixerContainer, Mixer);

Mixer::Mixer() : Resource()
{
	isLoop = false;
}

void Mixer::Load(const std::string& path)
{
	using namespace nlohmann;
	
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Mixer);
	}
	
	constexpr const char* Sounds = "Sounds";
	constexpr const char* Frequency = "Frequency";
	constexpr const char* MasterVolume = "MasterVolume";

	int fieldTracker = 0;
	try
	{
		json jsonFile;
		file >> jsonFile;

		std::string folder = "Asset/Audio/";
		std::string extention = ".wav";
		int index = 0;

		for (std::string sfilePath : jsonFile[Sounds])
		{
			std::wstring path = StringConverter::StrToWStr(sfilePath);
			_sounds.emplace_back(path);

			std::string name = sfilePath.erase(sfilePath.find(folder), folder.length());
			name = name.erase(name.find(extention), extention.length());

			_soundMap.insert(std::make_pair(name, index));
			index++;
		}

		fieldTracker++;

		auto freq = jsonFile[Frequency].get<float>();
		freqDist = std::normal_distribution(1.0f, freq);
		fieldTracker++;

		masterVolume = jsonFile[MasterVolume].get<float>();
	}
	catch (...)
	{
		std::ostringstream os;
		os << "[Field] ";
		switch (fieldTracker)
		{
		case 0:
			os << Sounds;
			break;
		case 1:
			os << Frequency;
			break;
		case 2:
			os << MasterVolume;
			break;
		}
		os << std::endl;
	
		os << "[Exception] Field doesn't have the right format";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Mixer);
	}
}

int Mixer::GetIndexPosition(std::string name)
{
	try
	{
		auto it = _soundMap.find(name);
		return it->second;
	}
	catch (const std::exception& e)
	{
		throw e.what();
	}
}

PChannel& Mixer::PlaySoundAt(float vol, int pos)
{
	if (pos >= _sounds.size() || pos < 0)
	{
		PChannel a = nullptr;
		return a;
	}

	return _sounds[pos].Play(1.0f, vol * masterVolume);
}

std::vector<PChannel> Mixer::PlayAllSounds(float vol)
{
	std::vector<PChannel> result;

	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		result.push_back((*i).Play(freqDist(rng), vol * masterVolume));
	}

	return result;
}

void Mixer::ResumeChannelInSoundAt(int pos, PChannel& chan)
{
	if (pos >= _sounds.size() || pos < 0 || !chan)
		return;

	_sounds[pos].ResumeChannel(chan);
}

void Mixer::ResumeSoundAt(int pos)
{
	if (pos >= _sounds.size() || pos < 0)
		return;

	_sounds[pos].ResumeSound();
}

void Mixer::ResumeAllSounds()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).ResumeSound();
	}
}

void Mixer::PauseChannelInSoundAt(int pos, PChannel& chan)
{
	if (pos >= _sounds.size() || pos < 0 || !chan)
		return;

	_sounds[pos].PauseChannel(chan);
}

void Mixer::PauseSoundAt(int pos)
{
	if (pos >= _sounds.size() || pos < 0)
		return;

	_sounds[pos].PauseSound();
}

void Mixer::PauseAllSounds()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).PauseSound();
	}
}

void Mixer::StopChannelInSoundAt(int pos, PChannel& chan)
{
	if (pos >= _sounds.size() || pos < 0 || !chan)
		return;

	_sounds[pos].StopChannel(chan);
}

void Mixer::StopSoundAt(int pos)
{
	if (pos >= _sounds.size() || pos < 0)
		return;

	_sounds[pos].StopSound();
}

void Mixer::StopAllSounds()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).StopSound();
	}
}

void Mixer::ChangeVolumeChannelInSoundAt(float vol, int pos, PChannel& chan)
{
	if (pos >= _sounds.size() || pos < 0 || !chan)
		return;

	_sounds[pos].ChangeVolumeChannel(vol, chan);
}

void Mixer::ChangeVolumeSoundAt(float vol, int pos)
{
	if (pos >= _sounds.size() || pos < 0)
		return;

	_sounds[pos].ChangeVolumeSound(vol * masterVolume);
}

void Mixer::ChangeVolumeAllSounds(float vol)
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).ChangeVolumeSound(vol * masterVolume);
	}
}

bool Mixer::IsChannelPlayingInSoundAt(int pos, PChannel chan)
{
	if (pos >= _sounds.size() || pos < 0 || !chan)
		return false;

	return _sounds[pos].IsChannelPlaying(chan);
}

bool Mixer::IsSoundPlayingAt(int pos)
{
	if (pos >= _sounds.size() || pos < 0)
		return false;

	return _sounds[pos].IsSoundPlaying();
}

bool Mixer::IsAllSoundsPlaying()
{
	if (_sounds.size() == 0)
		return false;

	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		if (!(*i).IsSoundPlaying())
			return false;
	}

	return true;
}

void Mixer::ChangeMasterVolume(float vol)
{
	masterVolume = vol;
}

float Mixer::GetMasterVolume()
{
	return masterVolume;
}

MixerContainer::MixerContainer()
{
	_name = RESOURCE_NAME(Mixer);
	LoadEntries(SystemPath::AudioEntriesPath);
}
