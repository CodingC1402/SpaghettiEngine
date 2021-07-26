#pragma once
#include "ScriptBase.h"
#include "CRectangle.h"

class SoundTriggerBox : public ScriptBase
{
public:
	enum Music
	{
		HappyBirthday,
		JustFriends,
		Aporia
	};
public:
	void OnFixedUpdate() override;
	void OnDisabled() override;
	void Load(nlohmann::json& input) override;

	void PlayMusic();
private:
	// Trigger area
	std::vector<CRectangle> _triggerZones;

	Music _musicTrack;

	// Use to make sure that the script don't switch on and off when collide with player at the same time
	//bool _isPlayerInside = false;

	//static SoundTriggerBox* __currentScript;
	REGISTER_START(SoundTriggerBox);
};

