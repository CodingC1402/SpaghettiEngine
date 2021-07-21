#pragma once
#include "Render2DScriptBase.h"
#include "Animation.h"
#include "Animator.h"

class ElectricScript : public Render2DScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;

	void Draw(PCamera script) override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	BoolField _runningField;
	Animator* _animator = nullptr;

	SAnimation _elcAnim;
	SSprite _currentSprite;

	float _counter = 0.0f;
private:
	REGISTER_START(ElectricScript);
};