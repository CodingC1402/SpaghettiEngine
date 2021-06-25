#pragma once
#include <memory>

class Shape;

class Body2D;
typedef std::weak_ptr<Body2D> WBody2D;

class Collider2DBase;
typedef std::weak_ptr<Collider2DBase> WCollider2DBase;

class GameObj;
typedef std::weak_ptr<GameObj> WGameObj;

class CollideEvent
{
	friend class Collision;
public:
	[[nodiscard]] WBody2D GetBody() const noexcept;;
	[[nodiscard]] WGameObj GetGameObject() const noexcept;;

	[[nodiscard]] bool GetIsHandled() const noexcept;;
	[[nodiscard]] bool IsCollideWithTrigger() const noexcept;

	[[nodiscard]] Collider2DBase* GetCollideWithScript() const noexcept;
	[[nodiscard]] Collider2DBase* GetCollideScript() const noexcept;
	void SetIsHandled(bool handled) noexcept;

	CollideEvent(WBody2D collideWith);
	CollideEvent(Collider2DBase* collideScript, WBody2D collideWith, Collider2DBase* collideWithScript);
	CollideEvent() = default;
protected:
	void SetCollideWithScript(Collider2DBase* script) noexcept;
	void SetCollideScript(Collider2DBase* script) noexcept;

	void SetToTrigger() noexcept;
	void Reset(Collider2DBase* collideScript, WBody2D collideWith, Collider2DBase* collideWithScript);
protected:
	WBody2D _collideWith;
	bool _isCollideWithTrigger = false;
	bool _isCollisionHandled = false;

	Collider2DBase* _collideScript = nullptr; // The object that get called event own this script;
	Collider2DBase* _collideWithScript = nullptr;
};

