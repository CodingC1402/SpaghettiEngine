#include "GameObj.h"
#include "ScriptBase.h"

// Normal Event
void GameObj::OnStart()
{
	if (!_parent && GetOwner())
		GetOwner()->AddToRoot(this);

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		(*it)->OnStart();
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnStart();
	}

	if (!IsDisabled())
		OnEnabled();
}

void GameObj::OnUpdate()
{
	if (IsDisabled())
		return;

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		if (!(*it)->IsDisabled())
			(*it)->OnUpdate();
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnUpdate();
	}
}

void GameObj::OnPhysicUpdate()
{
	_physic.GetBody2D().lock()->SetWorldMatrix(_transform.GetWorldMatrix());
}

void GameObj::Destroy()
{
	if (_parent)
		_parent->GetChildContainer().RemoveItem(this);

	BaseComponent::Destroy();
}

void GameObj::OnFixedUpdate()
{
	if (IsDisabled())
		return;

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		if (!(*it)->IsDisabled())
			(*it)->OnFixedUpdate();
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnFixedUpdate();
	}
}

void GameObj::OnEnabled()
{
	if (IsDisabled())
		return;

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		if (!(*it)->IsDisabled())
			(*it)->OnEnabled();
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnEnabled();
		(*it)->SetParentDisability(false);
	}
}

void GameObj::OnDisabled()
{
	if (IsDisabled())
		return;

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		if (!(*it)->IsDisabled())
			(*it)->OnDisabled();
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnDisabled();
		(*it)->SetParentDisability(true);
	}
}

void GameObj::OnCollide(CollideEvent& e)
{
	if (IsDisabled())
		return;

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		if (!(*it)->IsDisabled())
			(*it)->OnCollide(e);
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnCollide(e);
	}
}

void GameObj::OnCollideEnter(CollideEvent& e)
{
	if (IsDisabled)
		return;

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		if (!(*it)->IsDisabled())
			(*it)->OnCollideEnter(e);
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnCollideEnter(e);
	}
}

void GameObj::OnCollideExit(CollideEvent& e)
{
	if (IsDisabled)
		return;

	auto scriptsContainer = _scripts.GetContainer();
	for (auto it = scriptsContainer.begin(); it != scriptsContainer.end(); ++it)
	{
		if (!(*it)->IsDisabled())
			(*it)->OnCollideExit(e);
	}

	auto childrenContainer = _children.GetContainer();
	for (auto it = childrenContainer.begin(); it != childrenContainer.end(); ++it)
	{
		(*it)->OnCollideExit(e);
	}
}