#include "PlayerScript.h"
#include "SegmentScript.h"
#include "SpawnPoint.h"
#include "SceneManager.h"

REGISTER_FINISH(PlayerScript, ScriptBase) {
};

void PlayerScript::OnEnabled()
{
    __currentInstance = std::dynamic_pointer_cast<PlayerScript>(GetSharedPtr());
}

void PlayerScript::OnDisabled()
{
    __currentInstance.reset();
}

void PlayerScript::Respawn()
{
    SegmentScript::SpawnAll();
    auto delta = SpawnPoint::GetSpawnPointScript()->GetSpawnPosition() - GetGameObject()->GetTransform().GetWorldTransform();
    GetGameObject()->GetTransform().Translate(delta);

    _live--;
    if (_live == 0)
    {
        _score = 0;
        SceneManager::CallReloadScene();
    }
}

GameObj* PlayerScript::GetCurrentPlayer()
{
    return __currentInstance.expired() ? nullptr : __currentInstance.lock()->GetGameObject();
}
