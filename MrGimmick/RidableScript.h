#pragma once
#include "ScriptBase.h"
#include "Debug.h"
#include "MoveScript.h"

class RidableScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;
	void OnCollide(CollideEvent& e) override;
	PScriptBase Clone() const override;

private:
	MoveScript* _moveScript = nullptr;
	GameObj* _ridingObj;
	bool _isStand = false;
	Vector3 _lastPosition;

	REGISTER_START(RidableScript);
};