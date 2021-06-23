#include "GameObj.h"
#include "ScriptBase.h"

// Collision Event
void GameObj::SendCollideExitEvent(CollideEvent& e)
{
	for (const auto& script : _scripts)
		script->OnCollideExit(e);
}
void GameObj::SendCollideEvent(CollideEvent& e)
{
	for (const auto& script : _scripts)
		script->OnCollide(e);
}
void GameObj::SendCollideEnterEvent(CollideEvent& e)
{
	for (const auto& script : _scripts)
		script->OnCollideEnter(e);
}

// Normal Event
void GameObj::OnStart()
{
	for (const auto& script : _scripts)
		script->OnStart();
}

void GameObj::OnUpdate()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script->OnUpdate();
}

void GameObj::OnPhysicUpdate()
{
	_physic.GetBody2D().lock()->SetWorldMatrix(GetWorldMatrix());
}

void GameObj::OnFixedUpdate()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script->OnFixedUpdate();

	for (const auto& child : _children)
		child->OnFixedUpdate();
}

void GameObj::OnEnd()
{
	for (const auto& child : _children)
		child->OnEnd();

	for (const auto& script : _scripts)
		script->OnEnd();
}

void GameObj::OnEnabled()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script->OnEnabled();

	for (const auto& child : _children)
		child->OnEnabled();
}

void GameObj::OnDisabled()
{
	if (_isDisabled)
		return;

	for (const auto& script : _scripts)
		script->OnDisabled();

	for (const auto& child : _children)
		child->OnDisabled();
}

void GameObj::OnCollide(CollideEvent& e)
{
	for (auto& script : _scripts)
		script->OnCollide(e);
}

void GameObj::OnCollideEnter(CollideEvent& e)
{
	for (auto& script : _scripts)
		script->OnCollideEnter(e);
}

void GameObj::OnCollideExit(CollideEvent& e)
{
	for (auto& script : _scripts)
		script->OnCollideExit(e);
}