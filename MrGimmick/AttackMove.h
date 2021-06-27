#pragma once
#include "ScriptBase.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "StarScript.h"
#include "RigidBody2D.h"

typedef std::shared_ptr<InputAll> SInputAll;

class AttackMove : public ScriptBase
{
public:
	AttackMove(PScene owner, bool isDisabled = false);

	void OnStart() override;
	void OnUpdate() override;
	void Load(nlohmann::json& input);
protected:
	PGameObj		_starPrefab = nullptr;

	RigidBody2D*	_rb = nullptr;
	StarScript*		_starScript = nullptr;

	Vector3			_appearOffSet;
	SInputAll		_attackKey;
private:
	REGISTER_START(AttackMove);
};

