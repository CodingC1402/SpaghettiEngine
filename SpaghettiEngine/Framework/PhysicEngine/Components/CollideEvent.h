#pragma once
#include <memory>
#include "Vector3.h"

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
	[[nodiscard]] Body2D* GetBody() const noexcept;;
	[[nodiscard]] GameObj* GetGameObject() const noexcept;;

	[[nodiscard]] bool GetIsHandled() const noexcept;;
	[[nodiscard]] bool IsCollideWithTrigger() const noexcept;
	[[nodiscard]] Vector3 GetNormal() const noexcept;

	[[nodiscard]] Collider2DBase* GetCollideWithScript() const noexcept;
	[[nodiscard]] Collider2DBase* GetCollideScript() const noexcept;
	void SetIsHandled(bool handled) noexcept;

	CollideEvent(Body2D* collideWith);
	CollideEvent(Collider2DBase* collideScript, Body2D* collideWith, Collider2DBase* collideWithScript);
	CollideEvent() = default;
protected:
	void SetCollideWithScript(Collider2DBase* script) noexcept;
	void SetCollideScript(Collider2DBase* script) noexcept;
	void SetNormal(const Vector3& normal) noexcept;

	void SetToTrigger() noexcept;
	void Reset(Collider2DBase* collideScript, Body2D* collideWith, Collider2DBase* collideWithScript, const Vector3& normal);
protected:
	Body2D* _collideWith;
	bool _isCollideWithTrigger = false;
	bool _isCollisionHandled = false;
	Vector3	_collideNormal;

	Collider2DBase* _collideScript = nullptr; // The object that get called event own this script;
	Collider2DBase* _collideWithScript = nullptr;
};

