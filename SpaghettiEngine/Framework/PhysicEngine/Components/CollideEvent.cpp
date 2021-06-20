#include "CollideEvent.h"
#include "Body2D.h"
#include "Collider2DBase.h"
#include "Shape.h"

WBody2D CollideEvent::GetBody() const
{
	return _collideWith->GetBody();
}

WCollider2DBase CollideEvent::GetScript() const
{
	return GetBody().lock()->GetColliderScript();
}

WGameObj CollideEvent::GetObject() const
{
	return GetScript().lock()->GetGameObject();
}

bool CollideEvent::GetIsHandled() const
{
	return _isCollisionHandled;
}

void CollideEvent::SetIsHandled(bool handled)
{
	_isCollisionHandled = handled;
}

CollideEvent::CollideEvent(Shape* collideWith)
{
	_collideWith = collideWith;
}
