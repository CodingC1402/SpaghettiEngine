#include "DeadAnimationScript.h"
#include "FieldNames.h"
#include "HealthScript.h"
#include "Graphics.h"
#include "GameTimer.h"
#include "SMath.h"

REGISTER_FINISH(DeadAnimationScript, Render2DScriptBase) {}

void DeadAnimationScript::OnStart()
{
	auto healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
	healthScript->AddToHealthEvent(
		[&](const int& health, const int& delta)
		{
			if (delta < 0)
				_isStarted = (health == 0);
		}
	);
	_currentFrame = _anim->GetSpriteOfFrame(_index);
}

void DeadAnimationScript::OnUpdate()
{
	if (!_isStarted)
		return;

	_timeCounter += GameTimer::GetDeltaTime();
	_animTimer += GameTimer::GetDeltaTime();

	_anim->Advance(_index, _animTimer);
	_currentFrame = _anim->GetSpriteOfFrame(_index);

	if (_timeCounter / _time >= 1.0f)
	{
		_isStarted = false;
		_index = 0;
		_animTimer = 0;
		_currentFrame = _anim->GetSpriteOfFrame(_index);
		_timeCounter = 0;
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
}
