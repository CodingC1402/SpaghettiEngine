#include "Animation.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include <fstream>

CONTAINER_REGISTER(AnimationContainer, Animation);

size_t Animation::GetNumberOfFrames() const noexcept
{
	return _frames.size();
}

SSprite Animation::GetSpriteOfFrame(const unsigned int& frame) const
{
	return _frames[frame].sprite;
}

void Animation::Advance(unsigned int& frame, float& time)
{
	UINT nextFrame = frame;
	while (time >= 0)
	{
		frame = nextFrame;
		nextFrame++;

		if (nextFrame == _frames.size())
		{
			if (isLoop)
				nextFrame = 0;
			else
				return;
		}

		time -= _frames[nextFrame].delay;
	}
	time += _frames[nextFrame].delay;
}

AnimationContainer::AnimationContainer()
{
	_name = RESOURCE_NAME(Animation);
	LoadEntries(SystemPath::AnimationEntriesPath);
}

Animation::Animation() : Resource()
{
	isLoop = false;
}
void Animation::Load(const std::string& path)
{
	using namespace nlohmann;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Animation);
	}

	static constexpr const char* Texture = "Texture";
	static constexpr const char* Loop = "Loop";
	static constexpr const char* Frames = "Frames";
	int fieldTracker = 0;
	try
	{
		json jsonFile;
		file >> jsonFile;

		auto textureID = jsonFile[Texture].get<CULL>();
		fieldTracker++;
		STexture texture = TextureContainer::GetInstance()->GetResource(textureID);
		for (Frame loadedFrame; const auto& frame : jsonFile[Frames])
		{
			static constexpr int SpriteIndex = 0;
			static constexpr int Delay = 1;
			
			loadedFrame.sprite = texture->GetSprite(frame[SpriteIndex].get<int>());
			loadedFrame.delay = frame[Delay].get<float>();
			_frames.push_back(loadedFrame);
		}
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
			os << Texture;
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
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Animation);
	}
}
