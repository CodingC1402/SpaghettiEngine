#pragma once
#include <string>

#include "Tag.h"

namespace Fields
{
	class Character
	{
	public:
		static constexpr auto _character			= "Character";
	};
	class Player
	{
	public:
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

	class Enemy
	{
	public:
		static constexpr auto _enemy = "Enemy";
	};

	class Input
	{
	public:
		static constexpr auto _moveLeft		= "MoveLeft";
		static constexpr auto _moveRight	= "MoveRight";
		static constexpr auto _moveUp		= "MoveUp";
		static constexpr auto _moveDown		= "MoveDown";
		static constexpr auto _jump			= "Jump";
		static constexpr auto _attack		= "Attack";

		static constexpr auto _enterDebug	= "EnterDebug";
		static constexpr auto _fullScreen	= "FullScreen";
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

		static constexpr auto _createTime		= "CreateTime";
		static constexpr auto _smallStarAnim	= "SmallStarAnim";
		static constexpr auto _radius			= "Radius";
		static constexpr auto _numberOfStar		= "NumberOfStar";
		static constexpr auto _spinAngle		= "SpinAngle";
	};

	class FeetScript
	{
	public:
		static constexpr auto _width = "Width";
		static constexpr auto _height = "Height";
		static constexpr auto _position = "Position";
	};

	class Platform
	{
	public:
		static constexpr auto _platform = "Platform";
	};

	class DebugControl
	{
	public:
		static constexpr auto _debugFlySpeed = "FlySpeed";
	};
	
	class SegmentTrigger
	{
	public:
		static constexpr auto _center = "Center";
		static constexpr auto _width = "Width";
		static constexpr auto _height = "Height";
	};

	class TubeScript
	{
	public:
		static constexpr auto _pointA = "PointA";
		static constexpr auto _pointB = "PointB";

		static constexpr auto _center = "Center";
		static constexpr auto _width = "Width";
		static constexpr auto _height = "Height";
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
		[[nodiscard]] static Tag& GetStarOrCharacterTag();
		
	private:
		static void Load();
	private:
		static inline Tag _platformTag;
		static inline Tag _characterTag;
		static inline Tag _playerTag;
		static inline Tag _headTag;
		static inline Tag _feetTag;
		static inline Tag _enemyTag;
		static inline Tag _starOrCharacter;

		static inline bool _isLoaded = false;
	};
}