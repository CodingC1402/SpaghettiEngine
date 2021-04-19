#include "Animation.h"
#include "json.hpp"
#include <fstream>

std::list<SAnimation> Animation::__loadedAnimation;

SAnimation Animation::GetAnimation(int index)
{
	auto it = __loadedAnimation.begin();
	std::advance(it, index);
	return *it;
}

SAnimation Animation::GetAnimation(const std::string& path)
{
	for (const auto& animation : __loadedAnimation)
	{
		if (animation->_path == path)
			return animation;
	}

	return LoadAnimation(path);
}

SAnimation Animation::LoadAnimation(const std::string& path)
{
	SAnimation newAnimation(new Animation(path));
	__loadedAnimation.push_back(newAnimation);
	return newAnimation;
}

int Animation::GetNumberOfFrames() const noexcept
{
	return _frames.size();
}

ItFrame Animation::Begin()
{
	return _frames.begin();
}

void Animation::Advance(ItFrame& itFrame, double& time)
{
	ItFrame nextFrame = itFrame;
	while (time >= 0)
	{
		itFrame = nextFrame;
		std::advance(nextFrame, 1);

		if (nextFrame == _frames.end())
		{
			if (isLoop)
				nextFrame = _frames.begin();
			else
				return;
		}

		time -= nextFrame->delay;
	}
	time += nextFrame->delay;
}

Animation::Animation(const std::string& path)
{
	_path = path;
}

#define TEXTUREPATH "TexturePath"
#define	FRAME "Frames"
#define LOOP "loop"
#define SPRITEINDEX 0
#define DELAY 1
void Animation::Load()
{
	using namespace nlohmann;

	std::ifstream file(_path);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << L"File ";
		os << _path.c_str();
		os << L" Doesn't exist";
		throw TEXTURE_EXCEPT(os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		std::string texturePath = jsonFile[TEXTUREPATH].get<std::string>();
		STexture texture;
		Texture::GetTexture(&texture, texturePath);
		Frame loadedFrame;
		for (const auto& frame : jsonFile[FRAME])
		{
			texture->GetSprite(&loadedFrame.sprite, frame[SPRITEINDEX].get<int>());
			_frames.push_back(loadedFrame);
		}
		this->isLoop = jsonFile[LOOP].get<bool>();
	}
	catch (...)
	{
		std::wostringstream os;
		os << L"File ";
		os << _path.c_str();
		os << L" doesn't have the right format";
		throw TEXTURE_EXCEPT(os.str());
	}
}

void Animation::RemoveAnimation(const std::string& path)
{
	for (auto it = __loadedAnimation.begin(); it != __loadedAnimation.end(); std::advance(it, 1))
	{
		if ((*it)->_path == path)
		{
			__loadedAnimation.erase(it);
			return;
		}
	}
}

void Animation::ClearUnusedAnimation()
{
	for (auto it = __loadedAnimation.begin(); it != __loadedAnimation.end(); std::advance(it, 1))
	{
		if ((*it).use_count() == 1)
		{
			auto eraseIt = it;
			std::advance(it, 1);
			__loadedAnimation.erase(eraseIt);
			return;
		}
	}
}

void Animation::ClearAnimation()
{
	__loadedAnimation.clear();
}
