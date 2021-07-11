#include "SoundSource.h"
#include "LoadingJson.h"

REGISTER_FINISH(SoundSource, ScriptBase) {}

void SoundSource::Load(nlohmann::json& inputObject)
{
	try
	{
		using LoadingJson::Field;

		std::string _filePath = inputObject["Audio"].get<std::string>();

		_audio = MixerContainer::GetInstance()->GetResource(10001);
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

void SoundSource::Play()
{
	_audio->PlayAt(1.0f, 0);
}

PScriptBase SoundSource::Clone() const
{
	auto clone = dynamic_cast<SoundSource*>(ScriptBase::Clone());

	//clone->_audio = 

	return clone;
}
