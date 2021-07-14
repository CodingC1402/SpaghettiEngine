#pragma once
#include "ScriptBase.h"
#include "MoveScript.h"

class FeetScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;

	PScriptBase Clone() const override;
protected:
	MoveScript* _moveScript = nullptr;
	bool _isGrounded		= false; // Because start on air
	bool _isLastGrounded	= false;

	Vector3 _position;
	float _width = 0.0f;
	float _height = 0.0f;
private:
	REGISTER_START(FeetScript);
};

