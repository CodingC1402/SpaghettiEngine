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
public:
	[[nodiscard]] WBody2D GetBody() const;
	[[nodiscard]] WGameObj GetGameObject() const;

	[[nodiscard]] bool GetIsHandled() const;
	void SetIsHandled(bool handled);

	CollideEvent(WBody2D collideWith);
protected:
	WBody2D _collideWith;
	bool _isCollisionHandled = false;
};

