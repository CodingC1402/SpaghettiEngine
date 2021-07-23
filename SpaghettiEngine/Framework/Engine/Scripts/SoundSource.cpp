#include "SoundSource.h"
#include "ScriptField.h"
#include "GameTimer.h"

REGISTER_FINISH(SoundSource, ScriptBase) {}

NLOHMANN_JSON_SERIALIZE_ENUM(SoundSource::PlayMode, {
{SoundSource::PlayMode::OVERLAP, "Overlap"},
{SoundSource::PlayMode::NOTHING, "Nothing"}
	})

void SoundSource::Load(nlohmann::json& inputObject)
{
	using Fields::SoundSource;
	int fieldTracker = 0;
	try
	{

		_audio = MixerContainer::GetInstance()->GetResource(inputObject[SoundSource::GetMixerField()].get<CULL>());
		fieldTracker++;

		string name = inputObject[SoundSource::GetSoundNameField()].get<std::string>();
		index = _audio->GetIndexPosition(name);
		fieldTracker++;
		
		mode = inputObject[SoundSource::GetModeField()].get<PlayMode>();
		fieldTracker++;

		isRepeat = inputObject[SoundSource::GetIsLoopField()].get<bool>();
		fieldTracker++;

		delay = inputObject[SoundSource::GetDelayField()].get<float>();
		fieldTracker++;

		volume = inputObject[SoundSource::GetVolumeField()].get<float>();
	}
	catch (const CornException& e)
	{
		std::ostringstream os;
		os << "[Field] ";
		switch (fieldTracker)
		{
		case 0:
			os << SoundSource::GetMixerField();
			break;
		case 1:
			os << SoundSource::GetSoundNameField();
			break;
		case 2:
			os << SoundSource::GetModeField();
			break;
		case 3:
			os << SoundSource::GetIsLoopField();
			break;
		case 4:
			os << SoundSource::GetDelayField();
			break;
		case 5:
			os << SoundSource::GetVolumeField();
			break;
		}
		os << std::endl;

		os << "[Exception] Field doesn't have the right format";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), SoundSource);
	}
	catch (const std::exception& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, StringConverter::StrToWStr(e.what()));
	}
	ScriptBase::Load(inputObject);
}

void SoundSource::OnUpdate()
{
	if (isRepeat && !_audio->IsChannelPlayingInSoundAt(index, _channel) && !isStop)
	{
		currentTime += GameTimer::GetDeltaTime();
	}

	if (currentTime > delay)
	{
		Play();
		currentTime = 0;
	}
}

void SoundSource::OnDisabled()
{
	Stop();
	_channel = nullptr;
	ScriptBase::OnDisabled();
}

void SoundSource::Play()
{
	isStop = false;
	
	if (!_channel)
	{
		_channel = _audio->PlaySoundAt(volume, index);
		return;
	}

	switch (mode)
	{
	case OVERLAP:
		_audio->StopChannelInSoundAt(index, _channel);
		_channel = _audio->PlaySoundAt(volume, index);
		break;
	case NOTHING:
		if (!_audio->IsChannelPlayingInSoundAt(index, _channel))
		{
			_channel = _audio->PlaySoundAt(volume, index);
		}
		break;
	default:
		break;
	}

}

void SoundSource::Stop()
{
	isStop = true;
	currentTime = 0;
	_audio->StopChannelInSoundAt(index, _channel);
}

void SoundSource::Pause()
{
	isStop = true;
	_audio->PauseChannelInSoundAt(index, _channel);
}

void SoundSource::Resume()
{
	isStop = false;
	_audio->ResumeChannelInSoundAt(index, _channel);
}

bool SoundSource::IsRepeat()
{
	return isRepeat;
}

void SoundSource::SetRepeat(bool r)
{
	isRepeat = r;
}

float SoundSource::GetDelay()
{
	return delay;
}

void SoundSource::SetDelay(float d)
{
	delay = d;
}

float SoundSource::GetVolume()
{
	return volume;
}

void SoundSource::SetVolume(float v)
{
	volume = v;
}

SoundSource::PlayMode SoundSource::GetMode()
{
	return mode;
}

void SoundSource::SetMode(PlayMode m)
{
	mode = m;
}

PScriptBase SoundSource::Clone() const
{
	auto clone = dynamic_cast<SoundSource*>(ScriptBase::Clone());

	clone->_audio = _audio;
	clone->index = index;
	clone->isRepeat = isRepeat;
	clone->isStop = isStop;
	clone->delay = delay;
	clone->volume = volume;

	return clone;
}
