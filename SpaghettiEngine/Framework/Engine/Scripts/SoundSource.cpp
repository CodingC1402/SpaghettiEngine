#include "SoundSource.h"
#include "LoadingJson.h"
#include "GameTimer.h"

REGISTER_FINISH(SoundSource, ScriptBase) {}

void SoundSource::Load(nlohmann::json& inputObject)
{
	constexpr const char* ID = "ID";
	constexpr const char* Name = "Name";
	constexpr const char* Loop = "Loop";
	constexpr const char* Delay = "Delay";
	constexpr const char* Volume = "Volume";

	int fieldTracker = 0;
	try
	{
		using LoadingJson::Field;

		_audio = MixerContainer::GetInstance()->GetResource(inputObject[ID].get<CULL>());
		fieldTracker++;

		string name = inputObject[Name].get<std::string>();
		index = _audio->GetIndexPosition(name);
		fieldTracker++;

		isRepeat = inputObject[Loop].get<bool>();
		fieldTracker++;

		delay = inputObject[Delay].get<float>();
		fieldTracker++;

		volume = inputObject[Volume].get<float>();
	}
	catch (const CornException& e)
	{
		std::ostringstream os;
		os << "[Field] ";
		switch (fieldTracker)
		{
		case 0:
			os << ID;
			break;
		case 1:
			os << Name;
			break;
		case 2:
			os << Loop;
			break;
		case 3:
			os << Delay;
			break;
		case 4:
			os << Volume;
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
	if (isRepeat && !_audio->IsPlayingAt(index) && !isStop)
	{
		currentTime += GameTimer::GetDeltaTime();
	}

	if (currentTime > delay)
	{
		_audio->PlayAt(volume, index);
		currentTime = 0;
	}
}

void SoundSource::Play()
{
	isStop = false;
	_audio->PlayAt(volume, index);
}

void SoundSource::Stop()
{
	isStop = true;
	currentTime = 0;
	_audio->StopAt(index);
}

void SoundSource::Pause()
{
	isStop = true;
	_audio->PauseAt(index);
}

void SoundSource::Resume()
{
	isStop = false;
	_audio->ResumeAt(index);
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
