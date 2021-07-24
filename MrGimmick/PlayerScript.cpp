#include "PlayerScript.h"
#include "SegmentScript.h"
#include "SpawnPoint.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Polygon2DCollider.h"

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
    if (health > 0)
    {
        if (delta < 0)
        {
            _isHurted.lock()->SetValue(true);
            _hurtCounter = _hurtTime;
            _control->Disable();
            _attackMoveScript->Disable();

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
    SegmentScript::DisableAllSegment();
    SegmentScript::SpawnAll();
    GetGameObject()->GetTransform().SetWorldTransform(SpawnPoint::GetSpawnPointScript()->GetSpawnPosition());
    auto segment = SpawnPoint::GetSpawnPointScript()->GetSegment();
    segment->Enable();

    EnableColliders();
    EnableRigidBody();
    _animator->Enable();

    _healthScript->SetHealth(_healthScript->GetMaxHealth());
    _live--;
    if (_live == 0)
    {
        _score = 0;
        SceneManager::CallReloadScene();
    }
}

void PlayerScript::DisableColliders()
{
    GetGameObject()->GetScriptContainer().IteratingWithLamda(
        [&](PScriptBase script) {
            if (script->GetType() == TYPE_NAME(Polygon2DCollider))
            {
                script->Disable();
            }
        }
    );
}

void PlayerScript::EnableColliders()
{
    GetGameObject()->GetScriptContainer().IteratingWithLamda(
        [&](PScriptBase script) {
            if (script->GetType() == TYPE_NAME(Polygon2DCollider))
            {
                script->Enable();
            }
        }
    );
}

void PlayerScript::DisableRigidBody()
{
    auto script = GetGameObject()->GetPhysicComponent().GetRigidBody2DScript();
    if (script)
        script->Disable();
}

void PlayerScript::EnableRigidBody()
{
    GetGameObject()->GetScriptContainer().IteratingWithLamda(
        [&](PScriptBase script) {
            if (script->GetType() == TYPE_NAME(RigidBody2D))
            {
                script->Enable();
            }
        }
    );
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
