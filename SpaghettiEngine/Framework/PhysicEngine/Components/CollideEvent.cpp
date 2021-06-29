#include "CollideEvent.h"
#include "Body2D.h"
#include "Collider2DBase.h"
#include "Shape.h"

Body2D* CollideEvent::GetBody() const noexcept
{
	return _collideWith;
}

Vector3 CollideEvent::GetNormal() const noexcept
{
	return _collideNormal;
}

GameObj* CollideEvent::GetGameObject() const noexcept
{
	return dynamic_cast<GameObj*>(GetBody()->GetGameObject()->GetSharedPtr().get());
}

bool CollideEvent::GetIsHandled() const noexcept
{
	return _isCollisionHandled;
}

bool CollideEvent::IsCollideWithTrigger() const noexcept
{
	return _isCollideWithTrigger;
}

Collider2DBase* CollideEvent::GetCollideWithScript() const noexcept
{
	return _collideWithScript;
}

Collider2DBase* CollideEvent::GetCollideScript() const noexcept
{
	return _collideScript;
}

void CollideEvent::SetIsHandled(bool handled) noexcept
{
	_isCollisionHandled = handled;
}

CollideEvent::CollideEvent(Body2D* collideWith)
{
	_collideWith = collideWith;
}

CollideEvent::CollideEvent(Collider2DBase* collideScript, Body2D* collideWith, Collider2DBase* collideWithScript)
{
	_collideWith = collideWith;
	_collideScript = collideScript;
	_collideWithScript = collideWithScript;
}

void CollideEvent::SetCollideWithScript(Collider2DBase* script) noexcept
{
	_collideWithScript = script;
}

void CollideEvent::SetCollideScript(Collider2DBase* script) noexcept
{
	_collideScript = script;
}

void CollideEvent::SetNormal(const Vector3& normal) noexcept
{
	_collideNormal = normal;
}

void CollideEvent::SetToTrigger() noexcept
{
	_isCollideWithTrigger = true;
}

void CollideEvent::Reset(Collider2DBase* collideScript, Body2D* collideWith, Collider2DBase* collideWithScript, const Vector3& normal)
{
	_collideWith = collideWith;
	_collideScript = collideScript;
	_collideWithScript = collideWithScript;
	_collideNormal = normal;
	_isCollisionHandled = false;
	_isCollideWithTrigger = false;
}