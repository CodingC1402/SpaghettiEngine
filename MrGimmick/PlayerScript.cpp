#include "PlayerScript.h"
#include "SegmentScript.h"
#include "SpawnPoint.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "HealthScript.h"

REGISTER_FINISH(PlayerScript, ScriptBase) {
};

void PlayerScript::OnStart()
{
    auto animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
    auto healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
    _rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
    _moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);

    healthScript->AddToEvent([&](const int& health, const int& delta) {
        this->TookDamage(health, delta);
     });
    _control = GET_FIRST_SCRIPT_OF_TYPE(PlayerControl);
    _isHurted = animator->GetField<bool>(Fields::Player::_isHurt);
}

void PlayerScript::OnEnabled()
{
    __currentInstance = std::dynamic_pointer_cast<PlayerScript>(GetSharedPtr());
}

void PlayerScript::OnDisabled()
{
    __currentInstance.reset();
}

void PlayerScript::OnUpdate()
{
    if (_hurtCounter > 0)
    {
        _hurtCounter -= GameTimer::GetDeltaTime();
        if (_hurtCounter <= 0)
        {
            _control->Enable();
            _isHurted.lock()->SetValue(false);
        }
    }
}

void PlayerScript::TookDamage(const int& health, const int& damage)
{
    _isHurted.lock()->SetValue(true);
    _hurtCounter = _hurtTime;
    _control->Disable();

    auto vel = _rb->GetVelocity();
    vel += _hurtVel * (_moveScript->IsFlipped() ? -1 : 1);
    _rb->SetVelocity(vel);
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

void PlayerScript::Load(nlohmann::json& input)
{
    _hurtTime = input[Fields::PlayerScript::_hurtTime].get<float>();
    _hurtVel = input[Fields::PlayerScript::_hurtVel];
}

GameObj* PlayerScript::GetCurrentPlayer()
{
    return __currentInstance.expired() ? nullptr : __currentInstance.lock()->GetGameObject();
}
