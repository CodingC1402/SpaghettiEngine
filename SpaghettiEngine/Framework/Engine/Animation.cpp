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

SAnimation Animation::GetAnimation(const std::string* path)
{
	for (const auto& animation : __loadedAnimation)
	{
		if (animation->_path == *path)
			return animation;
	}

	return LoadAnimation(path);
}

SAnimation Animation::LoadAnimation(const std::string* path)
{
	SAnimation newAnimation(new Animation(path));
	newAnimation->Load();
	__loadedAnimation.push_back(newAnimation);
	return newAnimation;
}

int Animation::GetNumberOfFrames() const noexcept
{
	return _frames.size();
}

SSprite Animation::GetSpriteOfFrame(const UINT* frame)
{
	return _frames[*frame].sprite;
}

void Animation::Advance(UINT* frame, double* time)
{
	UINT nextFrame = *frame;
	while (*time >= 0)
	{
		*frame = nextFrame;
		nextFrame++;

		if (nextFrame == _frames.size())
		{
			if (isLoop)
				nextFrame = 0;
			else
				return;
		}

		*time -= _frames[nextFrame].delay;
	}
	*time += _frames[nextFrame].delay;
}

Animation::Animation(const std::string* path)
{
	_path = *path;
}

#define TEXTUREPATH "TexturePath"
#define	FRAMES "Frames"
#define LOOP "Loop"
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
		for (const auto& frame : jsonFile[FRAMES])
		{
			texture->GetSprite(&loadedFrame.sprite, frame[SPRITEINDEX].get<int>());
			loadedFrame.delay = frame[DELAY].get<double>();
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

void Animation::RemoveAnimation(const std::string* path)
{
	for (auto it = __loadedAnimation.begin(); it != __loadedAnimation.end(); std::advance(it, 1))
	{
		if ((*it)->_path == *path)
		{
			__loadedAnimation.erase(it);
			return;
		}
	}
}

void Animation::ClearUnusedAnimation()
{
	size_t size = __loadedAnimation.size();
	auto iterator = __loadedAnimation.begin();
	while (size > 0)
	{
		if (iterator->use_count() <= 1)
		{
			auto eraseIterator = iterator;
			std::advance(iterator, 1);
			__loadedAnimation.erase(eraseIterator);
		}
		size--;
	}
}

void Animation::ClearAnimation()
{
	__loadedAnimation.clear();
}
