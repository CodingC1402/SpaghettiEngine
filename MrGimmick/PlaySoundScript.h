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
	SInputAll _play1Input;
	SInputAll _play2Input;
	SInputAll _stop1Input;
	SInputAll _stop2Input;

	int soundLayer = 0;

	SoundManager* _audio = nullptr;
private:
	REGISTER_START(PlaySoundScript);
};