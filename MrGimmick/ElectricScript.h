#pragma once
#include "ScriptBase.h"
#include "Animation.h"
#include "Animator.h"
#include "AIBTs.h"
#include "MoveScript.h"
#include "CRectangle.h"

class ElectricScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	WGameObj _currentStar;

	CRectangle _rect;
	BoolField _defendField;
	BoolField _crouchingField;

	MoveScript* _moveScript = nullptr;
	Animator* _electricAnimator = nullptr;
	Animator* _movingAnimator = nullptr;

	bool _counterStart = false;
	float _counter = 0;
	float _animationTime;

	float _waitTime = 1.0;
	float _time = 0;
private:
	REGISTER_START(ElectricScript);
};