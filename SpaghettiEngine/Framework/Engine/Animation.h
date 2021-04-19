#pragma once
#include <memory>
#include "Sprite.h"

typedef class Animation* PAnimation;
typedef std::shared_ptr<Animation> SAnimation;
typedef std::unique_ptr<Animation> UAnimation;

struct Frame
{
	SSprite sprite;
	double delay;
};
typedef std::list<Frame>::iterator ItFrame;

class Animation
{
public:
	static constexpr SAnimation GetAnimation(int index);
	static constexpr SAnimation GetAnimation(const std::string& path);
	static SAnimation LoadAnimation(const std::string& path);

	int GetNumberOfFrames() const noexcept;
	ItFrame Begin();

	// Take in iterator to frame and time passed, it will change to next frame
	// acordingly and return time left.
	void Advance(ItFrame &itFrame, double& time);
protected:
	Animation(const std::string& path);
	void Load();

	static void RemoveAnimation(const std::string& path);
	static void ClearUnusedAnimation();
	static void ClearAnimation();
protected:
	std::string _path;
	std::list<Frame> _frames;
	bool isLoop;

	static std::list<SAnimation> __loadedAnimation;
};