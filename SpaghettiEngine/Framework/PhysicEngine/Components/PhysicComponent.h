#pragma once
#include "Body2D.h"
#include <list>

class Collider2DBase;
class GameObj;

class PhysicComponent
{
public:
	class Component2D
	{
	public:
		Component2D(GameObj* owner);

		void ChangeBody(WBody2D body);
		void RemoveBody(WBody2D body);

		void CallOnChange();
		void Subscribe(Collider2DBase* collider);
		void UnSubscribe(Collider2DBase* collider);

		[[nodiscard]] WBody2D GetBody2D();
	protected:
		GameObj* _owner;
		SBody2D _body;
		std::list<Collider2DBase*> _colliders;
	};

	class Component3D
	{
	public:

	protected:
	};
public:
	void SubscribeTo2D(Collider2DBase* script);
	void UnSubscribeTo2D(Collider2DBase* script);
	
	void AddPhysicComponent(PhysicScriptBase* component);
	void RemovePhysicComponent(PhysicScriptBase* component);

	void Set2DBody(WBody2D body);
	void Remove2DBody(WBody2D body);
	WBody2D GetBody2D();
	PhysicComponent(GameObj* owner);
protected:
	std::deque<PhysicScriptBase*> _physicComponents;

	Component2D _2DComponents;
	Component3D _3DComponents;
	GameObj* _owner;
};