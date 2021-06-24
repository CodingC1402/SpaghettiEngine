#include "GameObj.h"
#include "ScriptBase.h"
#include "Body2D.h"

// Normal Event
void GameObj::OnStart()
{
	_scripts.IteratingWithLamda([](PScriptBase script) {
		script->OnStart();
	});

	_children.IteratingWithLamda([](PGameObj child) {
		child->OnStart();
	});
}

void GameObj::OnUpdate()
{
	if (IsDisabled())
		return;

	_scripts.IteratingWithLamda([](PScriptBase script) {
		if (!script->IsDisabled())
			script->OnUpdate();
	});

	_children.IteratingWithLamda([](PGameObj child) {
		child->OnUpdate();
	});
}

void GameObj::OnFixedUpdate()
{
	if (IsDisabled())
		return;

	_scripts.IteratingWithLamda([](PScriptBase script) {
		if (!script->IsDisabled())
			script->OnFixedUpdate();
	});

	_children.IteratingWithLamda([](PGameObj child) {
		child->OnFixedUpdate();
	});
}

void GameObj::OnEnabled()
{
	if (IsDisabled())
		return;

	_scripts.IteratingWithLamda([](PScriptBase script) {
		if (!script->IsDisabled())
			script->OnEnabled();
	});

	auto it = _children.Begin();
	bool result = _children.GetSize() == 0;
	while (!result)
	{
		(*it)->OnEnabled();
		(*it)->SetParentDisability(false);
		result = _children.Iterate(it);
	}
}

void GameObj::OnDisabled()
{
	if (IsDisabled())
		return;

	_scripts.IteratingWithLamda([](PScriptBase script) {
		if (!script->IsDisabled())
			script->OnDisabled();
		});

	auto it = _children.Begin();
	bool result = _children.GetSize() == 0;
	while (!result)
	{
		(*it)->OnDisabled();
		(*it)->SetParentDisability(true);
		result = _children.Iterate(it);
	}
}

void GameObj::OnCollide(CollideEvent& e)
{
	if (IsDisabled())
		return;

	_scripts.IteratingWithLamdaEvent([](PScriptBase script, CollideEvent& e) {
		if (!script->IsDisabled())
			script->OnCollide(e);
	}, e);

	_children.IteratingWithLamdaEvent([](PGameObj child, CollideEvent& e) {
		child->OnCollide(e);
	}, e);
}

void GameObj::OnCollideEnter(CollideEvent& e)
{
	if (IsDisabled())
		return;

	_scripts.IteratingWithLamdaEvent([](PScriptBase script, CollideEvent& e) {
		if (!script->IsDisabled())
			script->OnCollideEnter(e);
	}, e);

	_children.IteratingWithLamdaEvent([](PGameObj child, CollideEvent& e) {
		child->OnCollideEnter(e);
	}, e);
}

void GameObj::OnCollideExit(CollideEvent& e)
{
	if (IsDisabled())
		return;

	_scripts.IteratingWithLamdaEvent([](PScriptBase script, CollideEvent& e) {
		if (!script->IsDisabled())
			script->OnCollideExit(e);
	}, e);

	_children.IteratingWithLamdaEvent([](PGameObj child, CollideEvent& e) {
		child->OnCollideExit(e);
	}, e);
}

void GameObj::OnPhysicUpdate()
{
	_physic.GetBody2D().lock()->SetWorldMatrix(_transform.GetWorldMatrix());
}

void GameObj::Destroy()
{
	if (IsRoot())
		// Just remove from root if it's root.
		GetOwner()->RemoveFromRoot(this);
	else if (_parent)
		// Remove from the container of parent and remove from transform too but do nothing else.
		_parent->GetChildContainer().RemoveChildWithoutEvent(this);

	// Remove scripts
	_scripts.IteratingWithLamda([](PScriptBase script) {
		script->Disable();
		script->CallDestroy();
		});
	GetScriptContainer().RemoveAllItem();

	// Remove objects
	_children.IteratingWithLamda([](PGameObj child) {
		child->Disable();
		child->CallDestroy();
		});
	GetChildContainer().RemoveAllItem();

	BaseComponent::Destroy();
}