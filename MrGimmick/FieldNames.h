#pragma once
#include <string>

#include "Tag.h"

namespace Fields
{
	class AIScript {
	public:
		static constexpr auto _behaviorTree = "BehaviorTree";
		static constexpr auto _delay		= "Delay";
		static constexpr auto _changes		= "Changes";
	};
	class CameraBoundingBox
	{
	public:
		static constexpr auto _triggers = "Triggers";
		static constexpr auto _center	= "Center";
		static constexpr auto _width	= "Width";
		static constexpr auto _height	= "Height";

		static constexpr auto _maxX = "MaxX";
		static constexpr auto _minX = "MinX";
		static constexpr auto _maxY = "MaxY";
		static constexpr auto _minY = "MinY";
	};
	class Character
	{
	public:
		static constexpr auto _character = "Character";
	};
	class DropBombScript
	{
	public:
		static constexpr auto _width = "Width";
		static constexpr auto _height = "Height";
		static constexpr auto _center = "Center";
		static constexpr auto _levelRadius = "LevelRadius";
		static constexpr auto _totalFanLevel = "TotalFanLevel";
		static constexpr auto _explodeTime = "ExplodeTime";
	};
	class DebugControl
	{
	public:
		static constexpr auto _debugFlySpeed = "FlySpeed";
	};
	class ElectricEnemy {
	public:
		static constexpr auto _center = "Center";
		static constexpr auto _width = "Width";
		static constexpr auto _height = "Height";

		static constexpr auto _moving = "Moving";
		static constexpr auto _electric = "Electric";

		static constexpr auto _isDefend = "IsDefend";
		static constexpr auto _isCrouching = "IsCrouching";

		static constexpr auto _animationTime = "AnimationTime";
	};
	class EnemyJumpScript {
	public:
		static constexpr auto _center	= "Center";
		static constexpr auto _width	= "Width";
		static constexpr auto _height	= "Height";

		static constexpr auto _wall			= "Wall";
		static constexpr auto _ground		= "Ground";
		static constexpr auto _resetDelay	= "ResetDelay";
	};
	class Enemy
	{
	public:
		static constexpr auto _enemy	= "Enemy";
		static constexpr auto _ignore	= "Ignore";
	};
	class ElectricSpawn
	{
	public:
		static constexpr auto _doorWay1 = "DoorWay1";
		static constexpr auto _doorWay2 = "DoorWay2";
		static constexpr auto _spawn	= "Spawn";

		static constexpr auto _center	= "Center";
		static constexpr auto _width	= "Width";
		static constexpr auto _height	= "Height";
	};
	class FeetScript
	{
	public:
		static constexpr auto _width	= "Width";
		static constexpr auto _height	= "Height";
		static constexpr auto _position = "Position";
		static constexpr auto _groundTag= "GroundTag";
		static constexpr auto _tagMode	= "TagMode";
	};
	class FootEnemyScript
	{
	public:
		static constexpr auto _diedNode = "DiedNode";
		static constexpr auto _hurtVel	= "HurtVelocity";
		static constexpr auto _delay	= "Delay";
	};
	class HeadScript
	{
	public:
		static constexpr auto _center	= "Center";
		static constexpr auto _width	= "Width";
		static constexpr auto _height	= "Height";
	};
	class HealthScript
	{
	public:
		static constexpr auto _maxHealth	= "MaxHealth";
		static constexpr auto _health		= "Health";
		static constexpr auto _iFrame		= "IFrame";
		static constexpr auto _damageSource = "DamageSources";
		static constexpr auto _tag			= "Tag";
		static constexpr auto _damage		= "Damage";
		static constexpr auto _hitBoxes		= "HitBoxes";
		static constexpr auto _center		= "Center";
		static constexpr auto _width		= "Width";
		static constexpr auto _height		= "Height";
	};
	class Input
	{
	public:
		static constexpr auto _moveLeft = "MoveLeft";
		static constexpr auto _moveRight= "MoveRight";
		static constexpr auto _moveUp	= "MoveUp";
		static constexpr auto _moveDown = "MoveDown";
		static constexpr auto _jump		= "Jump";
		static constexpr auto _attack	= "Attack";

		static constexpr auto _enterDebug = "EnterDebug";
		static constexpr auto _fullScreen = "FullScreen";
	};
	class IFrameFlash
	{
	public:
		static constexpr auto _flashDelay = "Delay";
		static constexpr auto _flashAlpha = "Alpha";
	};
	class MovableObject {
	public:
		static constexpr auto _speed	= "Speed";
		static constexpr auto _path		= "Path";
		static constexpr auto _isLoop	= "IsLoop";
		static constexpr auto _delay	= "Delay";
	};
	class Player
	{
	public:
		static constexpr auto _playerAttack			= "PlayerAttack";
		static constexpr auto _player				= "Player";
		static constexpr auto _head					= "Head";
		static constexpr auto _feet					= "Feet";

		static constexpr auto _isInTube				= "IsInTube";
		static constexpr auto _isHurt				= "IsHurt";
		static constexpr auto _isGrounded			= "IsGrounded";
		static constexpr auto _isRunning			= "IsRunning";

		static constexpr auto _reduceSpeed			= "ReduceSpeed";
		static constexpr auto _speedCapField		= "SpeedCap";
		static constexpr auto _jumpStrengthField	= "JumpStrength";
		static constexpr auto _speedRamUpField		= "SpeedRamUp";
		static constexpr auto _gsDropFactor			= "GSDropFactor";
		static constexpr auto _gsMin				= "GSMin";

		static constexpr auto _appearOffSet			= "AppearOffSet";
	};
	class PlayerScript
	{
	public:
		static constexpr auto _hurtTime = "HurtTime";
		static constexpr auto _hurtVel = "HurtVelocity";
		static constexpr auto _respawnDelay = "RespawnDelay";
	};
	class Platform
	{
	public:
		static constexpr auto _platform = "Platform";
	};
	class RidablePlatformUpDown
	{
	public:
		static constexpr auto _delta = "Delta";
		static constexpr auto _delay = "Delay";
	};
	class DeadAnimationScript
	{
	public:
		static constexpr auto _count = "Count";
		static constexpr auto _time = "Time";
		static constexpr auto _distance = "Distance";
		static constexpr auto _animation = "Animation";
	};
	class Star
	{
	public:
		static constexpr auto _star				= "Star";
		static constexpr auto _explodeTime		= "ExplodeTime";
		static constexpr auto _explodeAnimTime	= "ExplodeAnimTime";
		static constexpr auto _createAnimTime	= "CreateAnimTime";
		static constexpr auto _beforeUsable		= "BeforeUsable";
		static constexpr auto _startVelocity	= "StartVelocity";
		static constexpr auto _maxDistance		= "MaxDistance";
		static constexpr auto _cancelZone		= "CancelZone";

		static constexpr auto _createTime		= "CreateTime";
		static constexpr auto _smallStarAnim	= "SmallStarAnim";
		static constexpr auto _radius			= "Radius";
		static constexpr auto _numberOfStar		= "NumberOfStar";
		static constexpr auto _spinAngle		= "SpinAngle";
	};
	class SegmentTrigger
	{
	public:
		static constexpr auto _center	= "Center";
		static constexpr auto _width	= "Width";
		static constexpr auto _height	= "Height";
	};
	class SegmentScript
	{
	public:
		static constexpr auto _enenySpawns	= "EnemySpawns";
		static constexpr auto _index		= "Index";
		static constexpr auto _position		= "Position";
	};

	class SpawnPoint
	{
	public:
		static constexpr auto _center		= "Center";
		static constexpr auto _width		= "Width";
		static constexpr auto _height		= "Height";
		static constexpr auto _spawnPoint	= "SpawnPoint";
	};
	class TubeScript
	{
	public:
		static constexpr auto _pointA	= "PointA";
		static constexpr auto _pointB	= "PointB";

		static constexpr auto _center	= "Center";
		static constexpr auto _width	= "Width";
		static constexpr auto _height	= "Height";

		static constexpr auto _path		= "Path";
		static constexpr auto _speed	= "Speed";
	};

	class SpecialTag
	{
	public:
		[[nodiscard]] static Tag& GetPlatformTag();
		[[nodiscard]] static Tag& GetCharacterTag();
		[[nodiscard]] static Tag& GetPlayerTag();
		[[nodiscard]] static Tag& GetHeadTag();
		[[nodiscard]] static Tag& GetFeetTag();
		[[nodiscard]] static Tag& GetEnemyTag();
		[[nodiscard]] static Tag& GetStaticEnemyTag();
		[[nodiscard]] static Tag& GetPlayerAttack();
	private:
		static void Load();
	private:
		static inline Tag _platformTag;
		static inline Tag _characterTag;
		static inline Tag _playerTag;
		static inline Tag _headTag;
		static inline Tag _feetTag;
		static inline Tag _enemyTag;
		static inline Tag _playerAttack;
		static inline Tag _staticEnemyTag;

		static inline bool _isLoaded = false;
	};
}