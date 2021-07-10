#include "PhysicComponent.h"
#include "Physic.h"
#include "Collider2DBase.h"
#include "ContainerUtil.h"
#include "RigidBody2D.h"

PhysicComponent::Component2D::Component2D(GameObj* owner)
{
	_body = std::make_shared<Body2D>();
	_owner = owner;
	_body->SetGameObject(owner);
}

void PhysicComponent::Component2D::ChangeBody(RigidBody2D* script)
{
	if (script == nullptr)
		return;
	if (_body == script->GetBody())
		return;

	_body = script->GetBody();
	_body->SetGameObject(_owner);
	_bodyScript = script;
	CallOnChange();
}

void PhysicComponent::Component2D::RemoveBody(RigidBody2D* script)
{
	if (script == nullptr)
		return;

	if (_bodyScript == script)
	{
		_body = std::make_shared<Body2D>();
		_body->SetGameObject(_owner);
		Physic::AddBody(_body.get());
		_bodyScript = nullptr;

		CallOnChange();
	}
}

void PhysicComponent::Component2D::CallOnChange()
{
	for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
		(*it)->OnChange();
}

void PhysicComponent::Component2D::Subscribe(Collider2DBase* collider)
{
	_colliders.emplace(collider);
}

void PhysicComponent::Component2D::UnSubscribe(Collider2DBase* collider)
{
	_colliders.erase(collider);
}

RigidBody2D* PhysicComponent::Component2D::GetRigidBodyScript() const noexcept
{
	return _bodyScript;
}

WBody2D PhysicComponent::Component2D::GetBody2D() const noexcept
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

void PhysicComponent::AddPhysicComponent(PhysicScriptBase* component)
{
	if (!component)
		return;

	_physicComponents.emplace(component);
	Physic::AddGameObj(_owner);
}

void PhysicComponent::RemovePhysicComponent(PhysicScriptBase* component)
{
	if (!component)
		return;

	_physicComponents.erase(component);

	if (_physicComponents.empty())
		Physic::RemoveGameObj(_owner);
}

void PhysicComponent::Set2DBody(RigidBody2D* script)
{
	_2DComponents.ChangeBody(script);
}

void PhysicComponent::Remove2DBody(RigidBody2D* script)
{
	_2DComponents.RemoveBody(script);
}

WBody2D PhysicComponent::GetBody2D() const noexcept
{
	return _2DComponents.GetBody2D();
}

RigidBody2D* PhysicComponent::GetRigidBody2DScript() const noexcept
{
	return _2DComponents.GetRigidBodyScript();
}

PhysicComponent::PhysicComponent(GameObj* owner)
	:
	_2DComponents(owner)
{
	_owner = owner;
}
