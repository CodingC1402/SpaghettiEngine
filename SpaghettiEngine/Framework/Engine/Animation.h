#pragma once
#include <memory>
#include "ResourceContainer.h"
#include "Sprite.h"

typedef class Animation* PAnimation;
typedef std::shared_ptr<Animation> SAnimation;
typedef std::unique_ptr<Animation> UAnimation;

struct Frame
{
	SSprite sprite;
	double delay = 0;
};
typedef std::list<Frame>::iterator ItFrame;

class Animation : Resource
{
	friend class AnimationContainer;
protected:
	Animation(const std::string& path);

	void Load() override;
	void Advance(unsigned int& frame, float& time);
protected:
	std::vector<Frame> _frames;
	bool isLoop;
};

class AnimationContainer
{
	friend class SceneManager;
public:
	static SAnimation GetAnimation(int index);
	static SAnimation GetAnimation(const std::string& path);
	static SAnimation LoadAnimation(const std::string& path);

	[[nodiscard]]size_t GetNumberOfFrames() const noexcept;

	// Take in index to frame and time passed, it will change to next frame
	// accordingly and return time left.
	SSprite GetSpriteOfFrame(const UINT* frame);
protected:
	static void RemoveAnimation(const std::string* path);
	static void ClearUnusedAnimation();
	static void ClearAnimation();
protected:
	static std::list<SAnimation> __loadedAnimation;
};