#include "AudioPlayer.h"
#include "LoadingJson.h"

REGISTER_FINISH(AudioPlayer, ScriptBase) {}

void AudioPlayer::Load(nlohmann::json& inputObject)
{
	try
	{
		using LoadingJson::Field;

		std::string _filePath = inputObject["Audio"].get<std::string>();

		_audio = AudioContainer::GetInstance()->GetResource(10001);
		//_audio->Load(_filePath);
	}
	catch (const CornException& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, e.What());
	}
	catch (const std::exception& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, StringConverter::StrToWStr(e.what()));
	}
	ScriptBase::Load(inputObject);
}

void AudioPlayer::Play()
{
	_audio->PlayRandom(1.0f);
}

PScriptBase AudioPlayer::Clone() const
{
	auto clone = dynamic_cast<AudioPlayer*>(ScriptBase::Clone());

	//clone->_audio = 

	return clone;
}
