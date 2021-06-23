#include "GameObj.h"
#include "ScriptBase.h"

// Normal Event
void GameObj::OnStart()
{
	for (const auto& script : _scripts)
		script.lock()->OnStart();
}

void GameObj::OnUpdate()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script.lock()->OnUpdate();
}

void GameObj::OnPhysicUpdate()
{
	_physic.GetBody2D().lock()->SetWorldMatrix(_transform.GetWorldMatrix());
}

void GameObj::Destroy()
{
	if (_parent)
		_parent->RemoveChild(this);

	BaseComponent::Destroy();
}

void GameObj::OnFixedUpdate()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script.lock()->OnFixedUpdate();
}

void GameObj::OnEnabled()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script.lock()->OnEnabled();

	for (const auto& child : _children)
		child.lock()->OnEnabled();
}

void GameObj::OnDisabled()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script.lock()->OnDisabled();

	for (const auto& child : _children)
		child.lock()->OnDisabled();
}

void GameObj::OnCollide(CollideEvent& e)
{
	for (auto& script : _scripts)
		script.lock()->OnCollide(e);

	for (const auto& child : _children)
		child.lock()->OnCollide(e);
}

void GameObj::OnCollideEnter(CollideEvent& e)
{
	for (auto& script : _scripts)
		script.lock()->OnCollideEnter(e);

	for (const auto& child : _children)
		child.lock()->OnCollideEnter(e);
}

void GameObj::OnCollideExit(CollideEvent& e)
{
	for (auto& script : _scripts)
		script.lock()->OnCollideExit(e);

	for (const auto& child : _children)
		child.lock()->OnCollideExit(e);
}