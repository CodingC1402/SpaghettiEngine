#include "Mixer.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include "StringConverter.h"
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
	constexpr const char* Loop = "Loop";

	int fieldTracker = 0;
	try
	{
		json jsonFile;
		file >> jsonFile;

		rng = std::mt19937(std::random_device()());
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
	catch (...)
	{

	}
}

void Mixer::PlayAt(float vol, int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Play(freqDist(rng), vol * masterVolume);
}

void Mixer::PlayRandom(float vol)
{
	_sounds[soundDist(rng)].Play(freqDist(rng), vol * masterVolume);
}

void Mixer::PlayAll(float vol)
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Play(freqDist(rng), vol * masterVolume);
	}
}

void Mixer::ContinueAt(int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Continue();
}

void Mixer::ContinueAll()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Continue();
	}
}

void Mixer::PauseAt(int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Pause();
}

void Mixer::PauseAll()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Pause();
	}
}

void Mixer::StopAt(int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].Stop();
}

void Mixer::StopAll()
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).Stop();
	}
}

void Mixer::ChangeVolumeAt(float vol, int pos)
{
	if (pos >= _sounds.size())
		return;

	_sounds[pos].ChangeVolume(vol * masterVolume);
}

void Mixer::ChangeVolumeAll(float vol)
{
	for (auto i = _sounds.begin(); i != _sounds.end(); i++)
	{
		(*i).ChangeVolume(vol * masterVolume);
	}
}

void Mixer::ChangeMasterVolume(float vol)
{
	masterVolume = vol;
}

MixerContainer::MixerContainer()
{
	_name = RESOURCE_NAME(Mixer);
	LoadEntries(SystemPath::AudioEntriesPath);
}
