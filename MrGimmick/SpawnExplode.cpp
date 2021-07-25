#include "SpawnExplode.h"
#include "HealthScript.h"
#include "LoadingJson.h"

REGISTER_FINISH(SpawnExplode, ScriptBase) {}

void SpawnExplode::OnStart()
{
	auto script = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
	script->AddToHealthEvent(
		[&](const int& health, const int& delta) {
			if (health == 0)
			{
				GetOwner()->Instantiate(_prefab, GetWorldTransform());
				GetGameObject()->Disable();
				GetGameObject()->CallDestroy();
			}
		}
	);
}

void SpawnExplode::Load(nlohmann::json& input)
{
	_prefab = GET_REF_OBJECT(0);
}

ScriptBase* SpawnExplode::Clone() const
{
	auto clone = dynamic_cast<SpawnExplode*>(ScriptBase::Clone());

	clone->_prefab = _prefab;

	return nullptr;
}
