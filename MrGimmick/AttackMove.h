#pragma once
#include "ScriptBase.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "StarScript.h"
#include "RigidBody2D.h"
#include "MoveScript.h"

typedef std::shared_ptr<InputAll> SInputAll;

class AttackMove : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void OnLateUpdate() override;
	void OnDisabled() override;

	void Load(nlohmann::json& input);
protected:
	PGameObj		_starPrefab = nullptr;
	bool			_throwed = false;
	WGameObj		_currentStar;

	RigidBody2D*	_rb = nullptr;
	StarScript*		_starScript = nullptr;
	MoveScript*		_moveScript = nullptr;
	SoundManager*	_soundManager = nullptr;

	Vector3			_appearOffSet;
	SInputAll		_attackKey;
private:
	REGISTER_START(AttackMove);
};

