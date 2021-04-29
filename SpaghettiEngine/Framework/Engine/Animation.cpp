#include "Animation.h"
#include "json.hpp"
#include <fstream>

std::list<SAnimation> Container<Animation>::__loadedAnimation;

SAnimation AnimationContainer::GetAnimation(int index)
{
	auto it = __loadedAnimation.begin();
	std::advance(it, index);
	return *it;
}

SAnimation AnimationContainer::GetAnimation(const std::string& path)
{
	for (const auto& animation : __loadedAnimation)
	{
		if (animation->_path == path)
			return animation;
	}

	return LoadAnimation(path);
}

SAnimation AnimationContainer::LoadAnimation(const std::string& path)
{
	SAnimation newAnimation(new Animation(path));
	newAnimation->Load();
	__loadedAnimation.push_back(newAnimation);
	return newAnimation;
}

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

Animation::Animation(const std::string& path) : Resource(path)
{
	isLoop = false;
}
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
		static constexpr const char* TexturePath = "TexturePath";
		static constexpr const char* Loop = "Loop";
		static constexpr const char* Frames = "Frames";
		
		json jsonFile;
		file >> jsonFile;

		auto texturePath = jsonFile[TexturePath].get<std::string>();
		STexture texture;
		Texture::GetTexture(&texture, texturePath);
		Frame loadedFrame;
		for (const auto& frame : jsonFile[Frames])
		{
			static constexpr int SpriteIndex = 0;
			static constexpr int Delay = 1;
			
			texture->GetSprite(&loadedFrame.sprite, frame[SpriteIndex].get<int>());
			loadedFrame.delay = frame[Delay].get<double>();
			_frames.push_back(loadedFrame);
		}
		this->isLoop = jsonFile[Loop].get<bool>();
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

void AnimationContainer::RemoveAnimation(const std::string* path)
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

void AnimationContainer::ClearUnusedAnimation()
{
	size_t size = __loadedAnimation.size();
	auto iterator = __loadedAnimation.begin();
	while (size > 0)
	{
		if (iterator->use_count() <= 1)
		{
			const auto eraseIterator = iterator;
			std::advance(iterator, 1);
			__loadedAnimation.erase(eraseIterator);
		}
		size--;
	}
}

void AnimationContainer::ClearAnimation()
{
	__loadedAnimation.clear();
}
