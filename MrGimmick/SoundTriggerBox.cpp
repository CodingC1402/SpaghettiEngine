#include "SoundTriggerBox.h"
#include "DebugRenderer.h"
#include "FieldNames.h"
#include "PhysicCollide.h"
#include "PlayerScript.h"
#include "PlayerSound.h"
#include "Setting.h"

REGISTER_FINISH(SoundTriggerBox, ScriptBase) {}

// Convert enum from json
NLOHMANN_JSON_SERIALIZE_ENUM(SoundTriggerBox::Music, {
{SoundTriggerBox::Music::HappyBirthday, Fields::SoundManager::_happyBirthday},
{SoundTriggerBox::Music::JustFriends, Fields::SoundManager::_justFriends},
{SoundTriggerBox::Music::Aporia, Fields::SoundManager::_aporia}
	})

//SoundTriggerBox* SoundTriggerBox::__currentScript = nullptr;

constexpr unsigned TriggerZoneColor = 0xFFFF0000;

void SoundTriggerBox::OnFixedUpdate()
{
	//_isPlayerInside = false;
	std::set<GameObj*> objList;
	for (auto& rect : _triggerZones)
	{
		PhysicCollide::GetCollidedWithRectangle(
			GetGameObject(),
			objList,
			rect.GetCenter(),
			rect.GetWidth(),
			rect.GetHeight(),
			Fields::SpecialTag::GetPlayerTag(),
			PhysicCollide::FilterMode::Collide
		);
		if (!objList.empty())
		{
			PlayMusic();
			//_isPlayerInside = true;
			//if ((__currentScript && !__currentScript->_isPlayerInside) || !__currentScript)
			//	__currentScript = this;
			break;
		}
	}

	// Debug
	if constexpr (Setting::IsDebugMode())
	{
		for (auto& rect : _triggerZones)
		{
			DebugRenderer::DrawRectangleFromCenter(
				rect.GetCenter(),
				rect.GetWidth(),
				rect.GetHeight(),
				GetWorldMatrix(),
				TriggerZoneColor
			);
		}
	}
}

void SoundTriggerBox::OnDisabled()
{
	//if (__currentScript == this)
	//	__currentScript = nullptr;
}

void SoundTriggerBox::Load(nlohmann::json& input)
{
	Vector3 center;
	float	width;
	float	height;

	_musicTrack = input[Fields::SoundTriggerBox::_music].get<Music>();

	for (auto& trigger : input[Fields::SoundTriggerBox::_triggers])
	{
		center = trigger[Fields::SoundTriggerBox::_center];
		width = trigger[Fields::SoundTriggerBox::_width].get<float>();
		height = trigger[Fields::SoundTriggerBox::_height].get<float>();

		_triggerZones.push_back(CRectangle(center, width, height));
	}
}

void SoundTriggerBox::PlayMusic()
{
	auto soundPlayer = PlayerSound::GetCurrentPlayerSound();
	switch (_musicTrack)
	{
	case SoundTriggerBox::HappyBirthday:
		if (!soundPlayer->IsPlayingHappyBirthday())
		{
			soundPlayer->StopAllMusic();
			soundPlayer->PlayHappyBirthday();
		}
		break;
	case SoundTriggerBox::JustFriends:
		if (!soundPlayer->IsPlayingJustFriends())
		{
			soundPlayer->StopAllMusic();
			soundPlayer->PlayJustFriends();
		}
		break;
	case SoundTriggerBox::Aporia:
		if (!soundPlayer->IsPlayingAporia())
		{
			soundPlayer->StopAllMusic();
			soundPlayer->PlayAporia();
		}
		break;
	default:
		break;
	}
}
