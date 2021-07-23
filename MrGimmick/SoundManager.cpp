#include "SoundManager.h"
#include "FieldNames.h"

REGISTER_FINISH(SoundManager, ScriptBase) {}

void SoundManager::Load(nlohmann::json& input)
{

}

void SoundManager::OnUpdate()
{

}

PScriptBase SoundManager::Clone() const
{
	return PScriptBase();
}
