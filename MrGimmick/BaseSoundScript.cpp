#include "BaseSoundScript.h"
#include "FieldNames.h"

REGISTER_FINISH(BaseSoundScript, ScriptBase) {}

void BaseSoundScript::OnStart()
{
	_soundManager = GET_FIRST_SCRIPT_OF_TYPE(SoundManager);
}

PScriptBase BaseSoundScript::Clone() const
{
	auto clone = dynamic_cast<BaseSoundScript*>(ScriptBase::Clone());

	return clone;
}
