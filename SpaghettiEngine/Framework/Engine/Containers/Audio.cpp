#include "Audio.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include <fstream>

CONTAINER_REGISTER(AudioContainer, Audio);

Audio::Audio() : Resource()
{
	isLoop = false;
}

size_t Audio::GetNumberOfSounds() const noexcept
{
	return _sounds.size();
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
	constexpr const char* Loop = "Loop";
	constexpr const char* Frames = "Frames";
	int fieldTracker = 0;
	try
	{
		json jsonFile;
		file >> jsonFile;

		auto path = jsonFile[Sounds].get<std::wstring>();
		fieldTracker++;

		_sounds.emplace_back(path);

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
			os << Frames;
			break;
		case 2:
			os << Loop;
			break;
		}
		os << std::endl;

		os << "[Exception] Field doesn't have the right format";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Audio);
	}
}

AudioContainer::AudioContainer()
{
	_name = RESOURCE_NAME(Audio);
	LoadEntries(SystemPath::AudioEntriesPath);
}
