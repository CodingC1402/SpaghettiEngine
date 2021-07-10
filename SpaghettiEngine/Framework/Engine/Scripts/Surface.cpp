#include "Surface.h"
#include <functional>

REGISTER_FINISH(Surface, ScriptBase) {}

void Surface::OnCollide(CollideEvent& e)
{
	auto rb = e.GetGameObject()->GetPhysicComponent().GetRigidBody2DScript();
	if (!rb)
		return;

	_collideNow.emplace(rb);
}

void ChangeVel(const std::set<RigidBody2D*>& rbSet, float vel)
{
	Vector3 newVel;
	for (auto& rb : rbSet)
	{
		newVel = rb->GetVelocity();
		newVel.x += vel;
		rb->SetVelocity(newVel);
	}
}

void Surface::OnFixedUpdate()
{
	auto oldIt = _collided.begin();
	auto newIt = _collideNow.begin();

	std::set<RigidBody2D*> exit;
	std::set<RigidBody2D*> enter;

	while (oldIt != _collided.end() && newIt != _collideNow.end())
	{
		if (*oldIt == *newIt)
		{
			++oldIt;
			++newIt;
		}
		else if (*oldIt > *newIt)
		{
			enter.insert(*newIt);
			++newIt;
		}
		else if (*oldIt < *newIt)
		{
			exit.insert(*newIt);
			++oldIt;
		}
	}

	exit.insert(oldIt, _collided.end());
	enter.insert(newIt, _collideNow.end());

	ChangeVel(exit, -_increaseVel);
	ChangeVel(enter, _increaseVel);

	_collided = std::move(_collideNow);
	_collideNow.clear();
}