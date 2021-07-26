#include "DeadAnimationScript.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "GameTimer.h"
#include "Setting.h"
#include "SMath.h"

REGISTER_FINISH(DeadAnimationScript, Render2DScriptBase) {}

void DeadAnimationScript::OnStart()
{
	Reset();
}

void DeadAnimationScript::OnUpdate()
{
	if (!_isStarted)
		return;

	_timeCounter += GameTimer::GetDeltaTime();
	_animTimer += GameTimer::GetDeltaTime();

	if (_anim->Advance(_index, _animTimer))
	{
		_currentFrame = _anim->GetSpriteOfFrame(_index);
		_index -= 2;
	}
	else
		_currentFrame = _anim->GetSpriteOfFrame(_index);

	if (_timeCounter / _time >= 1.0f)
	{
		Reset();
		_currentLoop--;
		if (_currentLoop > 0)
			_isStarted = true;
	}
}

void DeadAnimationScript::Draw(PCamera cam)
{
	if (!_isStarted)
		return;

	Vector3 pos;
	auto lerpTime = _timeCounter / _time;
	pos.x = SMath::Lerp(0.0f, _distance, lerpTime);

	Graphics::SetSpriteTransform(cam->GetMatrix(GetWorldMatrix()));
	for (unsigned i = 0; i < _count; i++)
	{
		Graphics::DrawSprite(
			_currentFrame,
			_currentFrame->GetCenter(),
			pos
		);
		pos = pos * _rotationMatrix;
	}
}

void DeadAnimationScript::Load(nlohmann::json& input)
{
	_count = input[Fields::DeadAnimationScript::_count].get<unsigned>();
	float degree = 360.0f / _count;
	_rotationMatrix = SMath::GetZAxisRotateMatrix(degree);
	_time = input[Fields::DeadAnimationScript::_time].get<float>();
	_distance = input[Fields::DeadAnimationScript::_distance].get<float>();
	_anim = AnimationContainer::GetInstance()->GetResource(input[Fields::DeadAnimationScript::_animation].get<CULL>());
	_loopTime = input[Fields::DeadAnimationScript::_loopTime].get<unsigned>();
}

void DeadAnimationScript::Start()
{
	_isStarted = true;
	_currentLoop = _loopTime;
}

void DeadAnimationScript::Reset()
{
	_isStarted = false;
	_index = 0;
	_animTimer = 0;
	_currentFrame = _anim->GetSpriteOfFrame(_index);
	_timeCounter = 0;
}

ScriptBase* DeadAnimationScript::Clone() const
{
	auto clone = dynamic_cast<DeadAnimationScript*>(ScriptBase::Clone());

	clone->_count = _count;
	clone->_rotationMatrix = _rotationMatrix;
	clone->_time = _time;
	clone->_distance = _distance;
	clone->_anim = _anim;
	clone->_loopTime = _loopTime;

	return clone;
}
