#pragma once
#include "ScriptBase.h"
#include "Animation.h"
#include "Animator.h"
#include "SearchingLeaf.h"

class ElectricScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	WGameObj _currentStar;

	BoolField _runningField;
	Animator* _movingAnimator = nullptr;
	Animator* _electricAnimator = nullptr;

	SAnimation _elcAnim;
	SSprite _currentSprite;
	bool _animEnded = false;
	float _idleCounter = 0;

	unsigned _frame = 0;
	float _counter = 0.0f;
private:
	REGISTER_START(ElectricScript);
};