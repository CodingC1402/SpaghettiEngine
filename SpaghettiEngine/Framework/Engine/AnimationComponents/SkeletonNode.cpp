#include "SkeletonNode.h"
#include "AnimationTree.h"
#include "GameObj.h"
#include "SMath.h"
#include "GameTimer.h"

Node::State SkeletonNode::Tick()
{
	SAnimationTree tree = std::dynamic_pointer_cast<AnimationTree>(_tree.lock());
	State result = State::Running;

	if (!_isRunning)
	{
		_isRunning = true;
		_frameIndex = 0;

		auto& transform = tree->GetOwner()->GetTransform();
		auto& frame = _frames[_frameIndex];

		transform.SetWorldTransform(frame._transform);
		transform.SetWorldRotation(frame._rotation);
		transform.SetWorldScale(frame._scale);
		tree->SetColor(frame._color);
	}
	else
	{
		if (_frameIndex < _frames.size() - 1)
		{
			auto& transform = tree->GetOwner()->GetTransform();
			auto& frame = _frames[_frameIndex];
			auto& nextFrame = _frames[static_cast<long long>(_frameIndex) + 1];
			
			_counter += GameTimer::GetDeltaTime();
			float time = 0;
			if (frame._time > 0)
			{
				time = _counter / frame._time;

				transform.SetWorldTransform	(SMath::Lerp(frame._transform, nextFrame._transform, time));
				transform.SetWorldRotation	(SMath::Lerp(frame._rotation , nextFrame._rotation , time));
				transform.SetWorldScale		(SMath::Lerp(frame._rotation , nextFrame._rotation , time));

				auto alpha	= SMath::Lerp(frame._color.alpha, nextFrame._color.alpha, time);
				auto red	= SMath::Lerp(frame._color.red	, nextFrame._color.red	, time);
				auto green	= SMath::Lerp(frame._color.green, nextFrame._color.green, time);
				auto blue	= SMath::Lerp(frame._color.blue	, nextFrame._color.blue	, time);
				Color newColor(red, green, blue, alpha);
				tree->SetColor(newColor);
			}
			else
			{
				time = 1.0f;

				transform.SetWorldTransform(nextFrame._transform);
				transform.SetWorldRotation(nextFrame._rotation);
				transform.SetWorldScale(nextFrame._rotation);
				tree->SetColor(nextFrame._color);
			}


			if (time >= 1.0f)
			{
				_frameIndex++;
				_counter = 0;
			}
		}
		else
		{
			Reset();
			if (!_loop)
				result = State::Success;
		}
	}
	return result;
}

void SkeletonNode::OnInterrupted()
{
	Reset();
}

void SkeletonNode::CustomLoad(nlohmann::json& input)
{
	_loop = input[_loopField];
	Frame newFrame;
	for (auto& frame : input[_framesField])
	{
		newFrame._transform = frame[_transformField];
		newFrame._rotation	= frame[_rotationField];
		newFrame._scale		= frame[_scaleField];
		newFrame._color		= frame[_colorField];
		newFrame._time		= frame[_timeField];
		_frames.push_back(newFrame);
	}
}

void SkeletonNode::Reset()
{
	_frameIndex = 0;
	_counter = 0;
	_isRunning = false;
}

SNode SkeletonNode::Clone(WBTs tree)
{
	SSkeletonNode clone = std::dynamic_pointer_cast<SkeletonNode>(LeafNode::Clone(tree));

	clone->_counter = _counter;
	clone->_frameIndex = _frameIndex;
	clone->_frames = _frames;
	clone->_loop = _loop;
	clone->_isRunning = _isRunning;

	return clone;
}
