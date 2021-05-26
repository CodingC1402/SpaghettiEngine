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
	float delay = 0;
};
typedef std::list<Frame>::iterator ItFrame;

class Animation : public Resource
{
	friend class AnimationContainer;
public:
	Animation();
	
	[[nodiscard]] size_t GetNumberOfFrames() const noexcept;
	[[nodiscard]]SSprite GetSpriteOfFrame(const unsigned int& frame) const;
	void Load(const std::string& path) override;
	void Advance(unsigned int& frame, float& time);
protected:
	std::vector<Frame> _frames;
	bool isLoop;
};

class AnimationContainer : public Container<Animation>
{
	friend class SceneManager;
public:
	AnimationContainer();
};