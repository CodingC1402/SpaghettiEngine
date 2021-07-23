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
		// store old frame if needed and start animation
		StartAnimation(tree);
	}
	else
	{
		if (_frameIndex < _frames.size() - 1)
		{
			// Transiting to next frame
			TransitionToNextFrame(tree);
		}
		else
		{
			if (!_loop)
			{
				_frameIndex = 0;
				_counter = 0;
				result = State::Success;
				// Restore the frame before animation if isReset = true;
				RestoreOldFrame(tree);
			}
			else
			{
				// Transition to next frame which will loop back to beginning
				// To snap back to frame one then just use first frame time = 0
				TransitionToNextFrame(tree);
			}
		}
	}
	return result;
}

void SkeletonNode::OnInterrupted()
{
	if (!_isRunning)
		return;

	Reset();

	SAnimationTree tree = std::dynamic_pointer_cast<AnimationTree>(_tree.lock());
	RestoreOldFrame(tree);
}

void SkeletonNode::CustomLoad(nlohmann::json& input)
{
	_loop	 = input[_loopField].get<bool>();
	_isReset = input[_resetField].get<bool>();
	Frame newFrame;
	for (auto& frame : input[_framesField])
	{
		if (!frame[_transformField].empty())
		{
			newFrame._transform = std::make_shared<Vector3>(frame[_transformField]);
			_oldFrame._transform = std::make_shared<Vector3>();
		}
		if (!frame[_rotationField].empty())
		{
			newFrame._rotation = std::make_shared<Vector3>(frame[_rotationField]);
			_oldFrame._rotation = std::make_shared<Vector3>();
		}
		if (!frame[_scaleField].empty())
		{
			newFrame._scale = std::make_shared<Vector3>(frame[_scaleField]);
			_oldFrame._scale = std::make_shared<Vector3>();
		}
		if (!frame[_colorField].empty())
		{
			newFrame._color = std::make_shared<Color>(frame[_colorField]);
			_oldFrame._color = std::make_shared<Color>(0);
		}

		newFrame._time		= frame[_timeField].empty()		 ? 0		  : frame[_timeField].get<float>();
		newFrame._time		= newFrame._time < 0			 ? 0		  : newFrame._time;

		_frames.push_back(newFrame);
		newFrame.Reset();
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
	clone->_isReset = _isReset;

	return clone;
}

void SkeletonNode::TransitionToNextFrame(SAnimationTree tree)
{
	auto& transform = tree->GetOwner()->GetTransform();
	auto& frame = _frames[_frameIndex];
	auto& nextFrame = _frames[(static_cast<long long>(_frameIndex) + 1) % _frames.size()];

	_counter += GameTimer::GetDeltaTime();
	float time = 0;
	if (nextFrame._time > 0)
	{
		time = _counter / nextFrame._time;

		if (nextFrame._transform.use_count() > 0)
			transform.SetWorldTransform(SMath::Lerp(*frame._transform.get(), *nextFrame._transform.get(), time));
		if (nextFrame._rotation.use_count() > 0)
			transform.SetWorldRotation(SMath::Lerp(*frame._rotation.get(), *nextFrame._rotation.get(), time));
		if (nextFrame._scale.use_count() > 0)
			transform.SetWorldScale(SMath::Lerp(*frame._rotation.get(), *nextFrame._rotation.get(), time));

		if (nextFrame._color.use_count() > 0)
		{
			auto alpha = SMath::Lerp(frame._color->alpha, nextFrame._color->alpha, time);
			auto red = SMath::Lerp(frame._color->red, nextFrame._color->red, time);
			auto green = SMath::Lerp(frame._color->green, nextFrame._color->green, time);
			auto blue = SMath::Lerp(frame._color->blue, nextFrame._color->blue, time);
			Color newColor(red, green, blue, alpha);
			tree->SetColor(newColor);
		}
	}
	else
	{
		time = 1.0f;

		if (nextFrame._transform.use_count() > 0)
			transform.SetWorldTransform(*nextFrame._transform.get());
		if (nextFrame._rotation.use_count() > 0)
			transform.SetWorldRotation(*nextFrame._rotation.get());
		if (nextFrame._scale.use_count() > 0)
			transform.SetWorldScale(*nextFrame._rotation.get());
		if (nextFrame._color.use_count() > 0)
			tree->SetColor(*nextFrame._color.get());
	}


	if (time >= 1.0f)
	{
		_frameIndex++;
		_frameIndex %= _frames.size();
		_counter = 0;
	}
}

void SkeletonNode::StartAnimation(SAnimationTree tree)
{
	_isRunning = true;
	_frameIndex = 0;

	auto& transform = tree->GetOwner()->GetTransform();
	auto& frame = _frames[_frameIndex];
	StoreOldFrame(tree);

	if (frame._transform.use_count() > 0)
		transform.SetWorldTransform(*frame._transform.get());
	if (frame._rotation.use_count() > 0)
		transform.SetWorldRotation(*frame._rotation.get());
	if (frame._scale.use_count() > 0)
		transform.SetWorldScale(*frame._scale.get());
	if (frame._color.use_count() > 0)
		tree->SetColor(*frame._color.get());
}

void SkeletonNode::StoreOldFrame(SAnimationTree tree)
{
	if (_isReset)
	{
		auto& transform = tree->GetOwner()->GetTransform();

		if (_oldFrame._transform.use_count() > 0)
			_oldFrame._transform = std::make_shared<Vector3>(transform.GetWorldTransform());

		if (_oldFrame._rotation.use_count() > 0)
			_oldFrame._rotation = std::make_shared<Vector3>(transform.GetWorldRotation());

		if (_oldFrame._scale.use_count() > 0)
			_oldFrame._scale = std::make_shared<Vector3>(transform.GetWorldScale());

		if (_oldFrame._color.use_count() > 0)
			_oldFrame._color = std::make_shared<Color>(tree->GetColor());
	}
}

void SkeletonNode::RestoreOldFrame(SAnimationTree tree)
{
	if (_isReset)
	{
		auto& transform = tree->GetOwner()->GetTransform();

		if (_oldFrame._transform.use_count() > 0)
			transform.SetWorldTransform(*_oldFrame._transform.get());

		if (_oldFrame._rotation.use_count() > 0)
			transform.SetWorldRotation(*_oldFrame._rotation.get());

		if (_oldFrame._scale.use_count() > 0)
			transform.SetWorldScale(*_oldFrame._scale.get());

		if (_oldFrame._color.use_count() > 0)
			tree->SetColor(*_oldFrame._color.get());
	}
}

void SkeletonNode::Frame::Reset()
{
	_rotation.reset();
	_transform.reset();
	_scale.reset();
	_color.reset();
	_time = 0;
}
