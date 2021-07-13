#pragma once
#include "SoundSource.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "ScriptBase.h"

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

	SoundSource* _audio = nullptr;
private:
	REGISTER_START(PlaySoundScript);
};