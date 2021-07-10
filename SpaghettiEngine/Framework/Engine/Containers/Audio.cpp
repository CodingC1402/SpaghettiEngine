#include "Audio.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include "StringConverter.h"
#include <fstream>

CONTAINER_REGISTER(AudioContainer, Audio);

Audio::Audio() : Resource()
{
	isLoop = false;
}

void Audio::Load(const std::string& path)
{
	using namespace nlohmann;
	
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Audio);
	}
	
	constexpr const char* Sounds = "Sounds";
	constexpr const char* Frequency = "Frequency";
	constexpr const char* MasterVolume = "MasterVolume";
	constexpr const char* Loop = "Loop";

	int fieldTracker = 0;
	try
	{
		json jsonFile;
		file >> jsonFile;

		rng = std::mt19937(std::random_device()());

		auto tempPath = jsonFile[Sounds].get<std::string>();
		std::wstring path = StringConverter::StrToWStr(tempPath);
		_sounds.emplace_back(path);
		fieldTracker++;

		auto freq = jsonFile[Frequency].get<float>();
		freqDist = std::normal_distribution(1.0f, freq);
		fieldTracker++;

		masterVolume = jsonFile[MasterVolume].get<float>();
		fieldTracker++;

		this->isLoop = jsonFile[Loop].get<bool>();
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
		case 3:
			os << Loop;
			break;
		}
		os << std::endl;
	
		os << "[Exception] Field doesn't have the right format";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Audio);
	}
}

void Audio::PlayAt(float vol, int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Play(freqDist(rng), vol * masterVolume);
}

void Audio::PlayRandom(float vol)
{
	_sounds[soundDist(rng)].Play(freqDist(rng), vol * masterVolume);
}

void Audio::PlayAll(float vol)
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Play(freqDist(rng), vol * masterVolume);
	}
}

void Audio::ContinueAt(int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Continue();
}

void Audio::ContinueAll()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Continue();
	}
}

void Audio::PauseAt(int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Pause();
}

void Audio::PauseAll()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Pause();
	}
}

void Audio::StopAt(int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Stop();
}

void Audio::StopAll()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Stop();
	}
}

void Audio::ChangeVolumeAt(float vol, int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].ChangeVolume(vol * masterVolume);
}

void Audio::ChangeVolumeAll(float vol)
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).ChangeVolume(vol * masterVolume);
	}
}

void Audio::ChangeMasterVolume(float vol)
{
	masterVolume = vol;
}

AudioContainer::AudioContainer()
{
	_name = RESOURCE_NAME(Audio);
	LoadEntries(SystemPath::AudioEntriesPath);
}
