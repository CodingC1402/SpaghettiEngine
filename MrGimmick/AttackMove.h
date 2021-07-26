#pragma once
#include "ScriptBase.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "AttackScript.h"
#include "RigidBody2D.h"
#include "CRectangle.h"
#include "MoveScript.h"

typedef std::shared_ptr<InputAll> SInputAll;

class AttackMove : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;
	void OnLateUpdate() override;
	void OnDisabled() override;
	void ForceThrow();
	
	void Load(nlohmann::json& input);
protected:
	PGameObj		_starPrefab = nullptr;
	bool			_throwed = false;
	bool			_needPlatformCheck = false;
	WGameObj		_currentStar;

	CRectangle		_cancelZone;
	RigidBody2D*	_rb = nullptr;
	AttackScript*	_starScript = nullptr;
	MoveScript*		_moveScript = nullptr;

	Vector3			_appearOffSet;
	SInputAll		_attackKey;
	SInputAll		_specialKey;
private:
	REGISTER_START(AttackMove);
};

