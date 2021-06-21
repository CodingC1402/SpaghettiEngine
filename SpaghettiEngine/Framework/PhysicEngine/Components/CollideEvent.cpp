#include "CollideEvent.h"
#include "Body2D.h"
#include "Collider2DBase.h"
#include "Shape.h"

WBody2D CollideEvent::GetBody() const
{
	return _collideWith;
}

WGameObj CollideEvent::GetGameObject() const
{
	return std::dynamic_pointer_cast<GameObj>(GetBody().lock()->GetGameObject()->GetSharedPtr());
}

bool CollideEvent::GetIsHandled() const
{
	return _isCollisionHandled;
}

void CollideEvent::SetIsHandled(bool handled)
{
	_isCollisionHandled = handled;
}

CollideEvent::CollideEvent(WBody2D collideWith)
{
	_collideWith = collideWith;
}
