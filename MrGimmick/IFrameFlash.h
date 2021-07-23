#pragma once
#include "ScriptBase.h"
#include "Animator.h"
#include "HealthScript.h"

class IFrameFlash : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void Load(nlohmann::json& input) override;
	void StartFlash();
	void StopFlash();

	ScriptBase* Clone() const override;
private:
	bool _isFlashed = false;
	bool _isRunning = false;

	Byte _reducedAlpha = 0;
	float _flashDelay = 0.0f;
	float _flashDelayCounter = 0.0f;

	Animator* _animator;
	HealthScript* _healthScript;

	REGISTER_START(IFrameFlash);
};

