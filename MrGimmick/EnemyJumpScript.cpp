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
    Vector3 groundScan = _groundScanRect;
    Vector3 wallScan = _wallScanRect;
    if (_moveScript->IsFlipped())
    {
        groundScan.x *= -1;
        wallScan.x *= -1;
    }

    // Check for collide ground
    std::set<GameObj*> gameObjGroundList;
    PhysicCollide::GetCollidedWithRectangle(
        GetGameObject(),
        gameObjGroundList,
        groundScan,
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
        wallScan,
        _wallScanRectWidth,
        _wallScanRectHeight,
        Fields::SpecialTag::GetPlatformTag(),
        PhysicCollide::FilterMode::Equal
    );

    // if not on ground or hit a wall
    if (gameObjGroundList.empty() || !gameObjWallList.empty())
    {
        if (!_lastJump)
        {
            _moveScript->StopJump();
            _lastJump = true;
        }
        _moveScript->StartJump();
    }
    else
    {
        _moveScript->StopJump();
        _lastJump = false;
    }

    DebugRenderer::DrawRectangleFromCenter(groundScan, _groundScanRectWidth, _groundScanRectHeight, GetWorldMatrix(), DEBUG_COLOR);
    DebugRenderer::DrawRectangleFromCenter(wallScan, _wallScanRectWidth, _wallScanRectHeight, GetWorldMatrix(), DEBUG_COLOR);
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
}
