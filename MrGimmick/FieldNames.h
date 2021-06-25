#pragma once
#include <string>

namespace Fields
{
	static class Player
	{
	public:
		static constexpr auto _isInTube = "IsInTube";
		static constexpr auto _isHurt = "IsHurt";
		static constexpr auto _isGrounded = "IsGrounded";
		static constexpr auto _isRunning = "IsRunning";

		static constexpr auto _speedCapField = "SpeedCap";
		static constexpr auto _jumpStrengthField = "JumpStrength";
		static constexpr auto _speedRamUpField = "SpeedRamUp";
	};

	static class Input
	{
	public:
		static constexpr auto _moveLeft = "MoveLeft";
		static constexpr auto _moveRight = "MoveRight";
		static constexpr auto _jump = "Jump";
		static constexpr auto _attack = "Attack";

		static constexpr auto _fullScreen = "FullScreen";
	};

	static class Platform
	{
	public:
		static inline const std::string _platform = "Platform";
	};
}