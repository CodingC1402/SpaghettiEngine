#include "PhysicComponent.h"
#include "Physic.h"
#include "Collider2DBase.h"

PhysicComponent::Component2D::Component2D(GameObj* owner)
{
	_body = std::make_shared<Body2D>();
	_owner = owner;
	_body->SetGameObject(owner);
}

void PhysicComponent::Component2D::ChangeBody(WBody2D body)
{
	if (_body == body.lock())
		return;
	_body = body.lock();
	_body->SetGameObject(_owner);
	CallOnChange();
}

void PhysicComponent::Component2D::RemoveBody(WBody2D body)
{
	if (_body == body.lock())
	{
		_body = std::make_shared<Body2D>();
		_body->SetGameObject(_owner);
		Physic::AddBody(_body.get());
	}
	CallOnChange();
}

void PhysicComponent::Component2D::CallOnChange()
{
	for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
		(*it)->OnChange();
}

void PhysicComponent::Component2D::Subscribe(Collider2DBase* collider)
{
	for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
		if (*it == collider)
			return;
	_colliders.push_back(collider);
}

void PhysicComponent::Component2D::UnSubscribe(Collider2DBase* collider)
{
	for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
		if (*it == collider)
		{
			_colliders.erase(it);
			return;
		}
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

PhysicComponent::PhysicComponent(GameObj* owner)
	:
	_2DComponents(owner)
{
	_owner = owner;
}
