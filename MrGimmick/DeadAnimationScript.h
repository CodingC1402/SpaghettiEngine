#pragma once
#include "Render2DScriptBase.h"
#include "Animation.h"
#include "Camera.h"

class DeadAnimationScript : public Render2DScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void Draw(PCamera cam) override;
	void Load(nlohmann::json& input) override;
private:
	bool _isStarted = false;

	SAnimation _anim;
	SSprite _currentFrame;
	unsigned _index;
	float _animTimer;

	Matrix4 _rotationMatrix;
	unsigned _count;

	float _distance = 0.0f;
	float _time = 0.0f;
	float _timeCounter = 0.0f;
private:
	REGISTER_START(DeadAnimationScript);
};

