#include "SoundManager.h"
#include "FieldNames.h"
#include "PlayerSound.h"
REGISTER_FINISH(SoundManager, ScriptBase) {}

void SoundManager::OnStart()
{
	for (auto script : GetGameObject()->GetScriptContainer().GetAllItem())
	{
		if (script->GetType() == TYPE_NAME(SoundSource))
		{
			auto it = _soundSources.find(script->GetName());
			if (_soundSources.end() != it)
			{
				it->second = dynamic_cast<SoundSource*>(script);
			}
		}
	}
}

void SoundManager::OnDisabled()
{
	for (auto sound : _soundSources)
	{
		if (sound.second)
			sound.second->Disable();
	}
}

void SoundManager::Load(nlohmann::json& input)
{
	for (auto sound : input[Fields::SoundManager::_sounds])
	{
		SoundSource* s = dynamic_cast<SoundSource*>(GetOwner()->CreateScriptBase(Fields::SoundManager::_soundSource, false));
		s->Load(sound);
		s->SetName(sound[Fields::SoundManager::_soundName].get<std::string>());

		GetGameObject()->GetScriptContainer().AddItem(s);
		_soundSources.insert(std::make_pair(sound[Fields::SoundManager::_soundName].get<std::string>(), nullptr));
	}
}

void SoundManager::Disable(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->Disable();
}

void SoundManager::Enable(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->Enable();
}

void SoundManager::Play(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->Play();
}

void SoundManager::Stop(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->Stop();
}

void SoundManager::Pause(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->Pause();
}

void SoundManager::Resume(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->Resume();
}

bool SoundManager::IsRepeat(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		return s->IsRepeat();

	return NULL;
}

void SoundManager::SetRepeat(bool r, const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->SetRepeat(r);
}

float SoundManager::GetDelay(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		return s->GetDelay();

	return NULL;
}

void SoundManager::SetDelay(float d, const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->SetDelay(d);
}

float SoundManager::GetVolume(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		return s->GetVolume();

	return NULL;
}

void SoundManager::SetVolume(float v, const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->SetVolume(v);
}

SoundSource::PlayMode SoundManager::GetMode(const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		return s->GetMode();

	return SoundSource::PlayMode::NOTHING;
}

void SoundManager::SetMode(SoundSource::PlayMode m, const std::string& name)
{
	auto s = FindInMapByName(name);

	if (s)
		s->SetMode(m);
}

SoundSource* SoundManager::FindInMapByName(const std::string& name)
{
	auto it = _soundSources.find(name);

	if (it != _soundSources.end())
		return it->second;

	return nullptr;
}

PScriptBase SoundManager::Clone() const
{
	auto clone = dynamic_cast<SoundManager*>(ScriptBase::Clone());

	clone->_soundSources = _soundSources;

	return clone;
}
