#pragma once
#include "Node.h"
#include "Vector3.h"
#include "Color.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(SkeletonNode);

class SkeletonNode : public LeafNode
{
public:
	struct Frame
	{
		Vector3 _rotation;
		Vector3 _transform;
		Vector3 _scale;
		Color	_color;
		float _time;
	};
public:
	State Tick() override;

	void OnInterrupted() override;
	void CustomLoad(nlohmann::json& input) override;
	void Reset();

	SNode Clone(WBTs tree) override;
private:
	std::vector<Frame> _frames;
	float _counter = 0;
	bool _loop = false;
	bool _isRunning = false;
	unsigned _frameIndex = 0;

	static constexpr auto _framesField		= "Frames";
	static constexpr auto _transformField	= "Transform";
	static constexpr auto _rotationField	= "Rotation";
	static constexpr auto _scaleField		= "Scale";
	static constexpr auto _colorField		= "Color";
	static constexpr auto _timeField		= "Time";
	static constexpr auto _loopField		= "Loop";

	NODE_REGISTER(SkeletonNode);
};