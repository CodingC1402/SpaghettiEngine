#pragma once
#include "Node.h"
#include "Vector3.h"
#include "Color.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(SkeletonNode);
CLASS_FORWARD_DECLARATION(AnimationTree);
typedef std::shared_ptr<Vector3> SVector3;
typedef std::shared_ptr<Color> SColor;

class SkeletonNode : public LeafNode
{
public:
	struct Frame
	{
	public:
		void Reset();
	public:
		SVector3 _rotation;
		SVector3 _transform;
		SVector3 _scale;
		SColor	_color;
		float _time;
	};
public:
	State Tick() override;

	void OnInterrupted() override;
	void CustomLoad(nlohmann::json& input) override;
	void Reset();

	SNode Clone(WBTs tree) override;
private:
	void TransitionToNextFrame(SAnimationTree tree);
	void StartAnimation(SAnimationTree tree);

	void StoreOldFrame(SAnimationTree tree);
	void RestoreOldFrame(SAnimationTree tree);
private:
	std::vector<Frame> _frames;
	Frame		_oldFrame;
	float		_counter	= 0;
	bool		_loop		= false;
	bool		_isRunning	= false;
	bool		_isReset	= false;
	unsigned	_frameIndex = 0;

	static constexpr auto _framesField		= "Frames";
	static constexpr auto _transformField	= "Transform";
	static constexpr auto _rotationField	= "Rotation";
	static constexpr auto _scaleField		= "Scale";
	static constexpr auto _colorField		= "Color";
	static constexpr auto _timeField		= "Time";
	static constexpr auto _loopField		= "Loop";
	static constexpr auto _resetField		= "Reset";

	NODE_REGISTER(SkeletonNode);
};