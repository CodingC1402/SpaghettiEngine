#include "Shape.h"
#include "Physic.h"

WBody2D Shape::GetBody()
{
	return _body;
}

float Shape::GetRestitution()
{
	return _body.lock()->_material.lock()->_restitution;
}

float Shape::GetStaticFriction()
{
	return _body.lock()->_material.lock()->_staticFriction;
}

float Shape::GetDynamicFriction()
{
	return _body.lock()->_material.lock()->_dynamicFriction;
}

float Shape::GetGravity()
{
	return _body.lock()->_gravityScale * Physic::GetGravity();
}
