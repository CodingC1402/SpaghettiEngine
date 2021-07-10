#pragma once
#include "ScriptBase.h"
#include "Camera.h"
#include "InputSystem.h"
#include "RigidBody2D.h"
#include "AudioPlayer.h"

class MoveScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input);
	MoveScript(PScene owner);
	void OnStart() override;
	void OnUpdate() override;
protected:
	SInput up;
	SInput down;
	SInput left;
	SInput right;
	PCamera cam;

	float _speedCap = 150;
	float _jumpStrength = 200;
	float _speedRamUp = 700;

	WRigidBody2D _rigidBody;

	Vector3 move;
	bool isFlipped = false;

	AudioPlayer sound;
private:
	static constexpr auto SpeedCapField = "SpeedCap";
	static constexpr auto JumpStrengthField = "JumpStrength";
	static constexpr auto SpeedRamUpField = "SpeedRamUp";
private:
	REGISTER_START(MoveScript);
};

