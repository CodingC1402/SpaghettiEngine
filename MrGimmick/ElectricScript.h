#pragma once
#include "ScriptBase.h"
#include "Animation.h"
#include "Animator.h"
#include "AIBTs.h"
#include "MoveScript.h"

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
	SAIBTs _behaviorTree;

	BoolField _runningField;
	BoolField _defendField;

	MoveScript* _moveScript = nullptr;
	Animator* _electricAnimator = nullptr;

	bool _counterStart = false;
	float _counter = 0;
	float _animationTime;

	float _waitTime = 1.0;
	float _time = 0;
private:
	REGISTER_START(ElectricScript);
};