#include "EnemyJumpScript.h"
#include "DebugRenderer.h"
#include "DebugRenderer.h"
#include "PhysicCollide.h"
#include "FieldNames.h"
#include <set>

REGISTER_FINISH(EnemyJumpScript, ScriptBase) {};
constexpr unsigned DEBUG_COLOR = 0xFFFF0000;

void EnemyJumpScript::OnStart()
{
    _moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
}

void EnemyJumpScript::OnFixedUpdate()
{
    // Check for collide ground
    std::set<GameObj*> gameObjGroundList;
    PhysicCollide::GetCollidedWithRectangle(
        GetGameObject(),
        gameObjGroundList,
        _groundScanRect,
        _groundScanRectWidth,
        _groundScanRectHeight,
        Fields::SpecialTag::GetPlatformTag(),
        PhysicCollide::FilterMode::Equal
    );
    // Check for collide wall
    std::set<GameObj*> gameObjWallList;
    PhysicCollide::GetCollidedWithRectangle(
        GetGameObject(),
        gameObjWallList,
        _wallScanRect,
        _wallScanRectWidth,
        _wallScanRectHeight,
        Fields::SpecialTag::GetPlatformTag(),
        PhysicCollide::FilterMode::Equal
    );

    // if not on ground or hit a wall
    if (gameObjGroundList.empty() || !gameObjWallList.empty())
    {
        _jumpAgainAccumulative += Physic::GetStep();
        if (_jumpAgainAccumulative > _jumpAgainDelay)
        {
            _jumpAgainAccumulative = 0;
            _lastJump = false;
        }

        if (!_lastJump)
        {
            _moveScript->StopJump();
            _moveScript->StartJump();
            _lastJump = true;
        }
    }
    else
    {
        _jumpAgainAccumulative = 0;
        _moveScript->StopJump();
        _lastJump = false;
    }

    DebugRenderer::DrawRectangleFromCenter(_groundScanRect, _groundScanRectWidth, _groundScanRectHeight, GetWorldMatrix(), DEBUG_COLOR);
    DebugRenderer::DrawRectangleFromCenter(_wallScanRect, _wallScanRectWidth, _wallScanRectHeight, GetWorldMatrix(), DEBUG_COLOR);
}

ScriptBase* EnemyJumpScript::Clone() const
{
    auto clone = dynamic_cast<EnemyJumpScript*>(ScriptBase::Clone());
        
    clone->_groundScanRect = _groundScanRect;
    clone->_groundScanRectWidth = _groundScanRectWidth;
    clone->_groundScanRectHeight = _groundScanRectHeight;

    clone->_wallScanRect = _wallScanRect;
    clone->_wallScanRectHeight = _wallScanRectHeight;
    clone->_wallScanRectWidth = _wallScanRectWidth;

    clone->_jumpAgainDelay = _jumpAgainDelay;

    return clone;
}

void EnemyJumpScript::Load(nlohmann::json& input)
{
    auto& wall          = input[Fields::EnemyJumpScript::_wall];
    _wallScanRect       = wall[Fields::EnemyJumpScript::_center];
    _wallScanRectWidth  = wall[Fields::EnemyJumpScript::_width].get<float>();
    _wallScanRectHeight = wall[Fields::EnemyJumpScript::_height].get<float>();

    auto& ground            = input[Fields::EnemyJumpScript::_ground];
    _groundScanRect         = ground[Fields::EnemyJumpScript::_center];
    _groundScanRectWidth    = ground[Fields::EnemyJumpScript::_width].get<float>();
    _groundScanRectHeight   = ground[Fields::EnemyJumpScript::_height].get<float>();

    _jumpAgainDelay = input[Fields::EnemyJumpScript::_resetDelay].get<float>();
}
