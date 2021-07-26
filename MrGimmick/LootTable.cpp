#include "LootTable.h"
#include "LoadingJson.h"
#include "Pickups.h"

REGISTER_FINISH(LootTable, ScriptBase) {}

void LootTable::Load(nlohmann::json& input)
{
    for (int i = 0; i < 3; i++)
    {
        _prefabs.emplace(i, GET_REF_OBJECT(i));
    }
}

void LootTable::OnDisabled()
{
    if (__instance == this)
        __instance = nullptr;
}

void LootTable::OnEnabled()
{
    if (__instance == nullptr)
        __instance = this;
}

bool LootTable::SpawnRandom(Vector3 pos)
{
    unsigned num = rand() % 100;
    if (num <= 70)
    {
        return false;
    }

    num = rand() % _prefabs.size();
    auto instance = GetOwner()->Instantiate(_prefabs[num], pos);
    auto script = dynamic_cast<Pickups*>(instance->GetScriptContainer().GetItemType(TYPE_NAME(Pickups)));
    script->SetTime(15);
    return true;
}

LootTable* LootTable::GetInstance()
{
    return __instance;
}
