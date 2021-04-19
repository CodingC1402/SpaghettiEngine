#include "Animation.h"

SAnimation Animation::GetAnimation(int index)
{
	auto it = __loadedAnimation.begin();
	std::advance(it, index);
	return *it;
}

constexpr SAnimation Animation::GetAnimation(const std::string& path)
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

void Animation::Load()
{

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
