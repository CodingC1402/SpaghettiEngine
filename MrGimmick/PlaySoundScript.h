#pragma once
#include "SoundManager.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "ScriptBase.h"

// This script is for testing

typedef std::shared_ptr<InputAll> SInputAll;

class PlaySoundScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input);
	void OnStart() override;
	void OnUpdate() override;
protected:
	SInputAll _moveLeftInput;
	SInputAll _moveRightInput;
	SInputAll _jumpInput;
	SInputAll _attackInput;

	int soundLayer = 0;

	SoundManager* _audio = nullptr;
private:
	REGISTER_START(PlaySoundScript);
};