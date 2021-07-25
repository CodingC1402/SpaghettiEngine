#include "PlayerScript.h"
#include "SegmentScript.h"
#include "SpawnPoint.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Polygon2DCollider.h"
#include "PlayerScoreBoard.h"
#include "PlayerSound.h"

REGISTER_FINISH(PlayerScript, ScriptBase) {
};

void PlayerScript::OnStart()
{
    _animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
    _healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
    _rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
    _moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
    _attackMoveScript = GET_FIRST_SCRIPT_OF_TYPE(AttackMove);

    // Top now when it took damage;
    _healthScript->AddToHealthEvent([&](const int& health, const int& delta) {
        this->TookDamage(health, delta);
    });

    _control = GET_FIRST_SCRIPT_OF_TYPE(PlayerControl);
    _isHurted = _animator->GetField<bool>(Fields::Player::_isHurt);
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
    if (_respawnCounter > 0)
    {
        _respawnCounter -= GameTimer::GetDeltaTime();
        if (_respawnCounter <= 0)
            Respawn();
        return;
    }

    if (_hurtCounter > 0)
    {
        _hurtCounter -= GameTimer::GetDeltaTime();
        if (_hurtCounter <= 0)
        {
            _control->Enable();
            _attackMoveScript->Enable();
            _isHurted.lock()->SetValue(false);
        }
    }
}

void PlayerScript::OnCollide(CollideEvent& e)
{
    //if (e.GetGameObject()->GetTag() == Fields::SpecialTag::GetPlatformTag())
    //{
    //    PlayerSound::GetCurrentPlayerSound()->PlayLandingSound();
    //}
}

ScriptBase* PlayerScript::Clone() const
{
    auto clone = dynamic_cast<PlayerScript*>(ScriptBase::Clone());

    clone->_hurtTime = _hurtTime;
    clone->_hurtVel = _hurtVel;
    clone->_respawnDelay = _respawnDelay;

    return clone;
}

void PlayerScript::TookDamage(const int& health, const int& delta)
{
    if (delta < 0)
    {
        _control->Disable();
        _attackMoveScript->Disable();
    }

    if (health > 0)
    {
        if (delta < 0)
        {
            _isHurted.lock()->SetValue(true);
            _hurtCounter = _hurtTime;

            auto hurtVelCopy = _hurtVel;
            hurtVelCopy.x *= _moveScript->IsFlipped() ? -1 : 1;
            _rb->SetVelocity(hurtVelCopy);
        }
        else if (delta > 0)
        {
            // TO DO
        }
    }
    else
    {
        _respawnCounter = _respawnDelay;
        _animator->Disable();
        DisableColliders();
        DisableRigidBody();
    }
}

void PlayerScript::Respawn()
{
    PlayerScoreBoard::GetInstance()->DecreaseLive();
    if (PlayerScoreBoard::GetInstance()->GetLive() < 0)
        return;

    PlayerSound::GetCurrentPlayerSound()->PlayHB();

    SegmentScript::DisableAllSegment();
    SegmentScript::SpawnAll();

    GetGameObject()->GetTransform().SetWorldTransform(SpawnPoint::GetSpawnPointScript()->GetSpawnPosition());
    auto segment = SpawnPoint::GetSpawnPointScript()->GetSegment();
    segment->Enable();

    EnableColliders();
    EnableRigidBody();

    _animator->Enable();
    _control->Enable();
    _attackMoveScript->Enable();

    _healthScript->SetHealth(_healthScript->GetMaxHealth());
}

void PlayerScript::DisableColliders()
{
    auto scripts = GET_ALL_SCRIPTS_OF_TYPE(Polygon2DCollider);
    for (auto& script : scripts)
        script->Disable();
}

void PlayerScript::EnableColliders()
{
    auto scripts = GET_ALL_SCRIPTS_OF_TYPE(Polygon2DCollider);
    for (auto& script : scripts)
        script->Enable();
}

void PlayerScript::DisableRigidBody()
{
    auto scripts = GET_ALL_SCRIPTS_OF_TYPE(RigidBody2D);
    for (auto& script : scripts)
        script->Disable();
}

void PlayerScript::EnableRigidBody()
{
    auto scripts = GET_ALL_SCRIPTS_OF_TYPE(RigidBody2D);
    for (auto& script : scripts)
        script->Enable();
}

void PlayerScript::Load(nlohmann::json& input)
{
    _hurtTime = input[Fields::PlayerScript::_hurtTime].get<float>();
    _hurtVel = input[Fields::PlayerScript::_hurtVel];
    _respawnDelay = input[Fields::PlayerScript::_respawnDelay].get<float>();
}

GameObj* PlayerScript::GetCurrentPlayer()
{
    return __currentInstance.expired() ? nullptr : __currentInstance.lock()->GetGameObject();
}
