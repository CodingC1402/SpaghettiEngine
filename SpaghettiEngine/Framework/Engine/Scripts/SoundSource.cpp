#include "SoundSource.h"
#include "LoadingJson.h"

REGISTER_FINISH(SoundSource, ScriptBase) {}

void SoundSource::Load(nlohmann::json& inputObject)
{
	constexpr const char* ID = "ID";
	constexpr const char* Name = "Name";

	try
	{
		using LoadingJson::Field;

		_audio = MixerContainer::GetInstance()->GetResource(inputObject[ID].get<CULL>());
		string name = inputObject[Name].get<std::string>();
		index = _audio->GetIndexPosition(name);
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
	_audio->PlayAt(1.0f, index);
}

PScriptBase SoundSource::Clone() const
{
	auto clone = dynamic_cast<SoundSource*>(ScriptBase::Clone());

	//clone->_audio = 

	return clone;
}
