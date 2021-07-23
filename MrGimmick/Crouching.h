#pragma once
#include "ScriptBase.h"
#include "AIBTs.h"
#include "Animator.h"
#include "MoveScript.h"
#include "CRectangle.h"

class Crouching : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	CRectangle _rect;
	BoolField _crouchingField;
	BoolField _defendField;

	MoveScript* _moveScript = nullptr;
	Animator* _movingAnimator = nullptr;
	Animator* _electricAnimator = nullptr;
private:
	REGISTER_START(Crouching);
};