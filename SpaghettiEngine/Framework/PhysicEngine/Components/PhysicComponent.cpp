#include "PhysicComponent.h"
#include "Collider2DBase.h"

void PhysicComponent::Component2D::ChangeBody(WBody2D body)
{
	if (_body == body.lock())
		return;
	_body = body.lock();
	CallOnChange();
}

void PhysicComponent::Component2D::RemoveBody(WBody2D body)
{
	if (_body == body.lock())
		_body = std::make_shared<Body2D>();
	CallOnChange();
}

WBody2D PhysicComponent::Component2D::GetBody2D()
{
	return _body;
}

void PhysicComponent::SubscribeTo2D(Collider2DBase* script)
{
	_2DComponents.Subscribe(script);
}

void PhysicComponent::UnSubscribeTo2D(Collider2DBase* script)
{
	_2DComponents.UnSubscribe(script);
}

void PhysicComponent::Set2DBody(WBody2D body)
{
	_2DComponents.ChangeBody(body);
}

void PhysicComponent::Remove2DBody(WBody2D body)
{
	_2DComponents.RemoveBody(body);
}

WBody2D PhysicComponent::GetBody2D()
{
	return _2DComponents.GetBody2D();
}
