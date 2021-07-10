#pragma once
#include "Macros.h"

#include <set>

CLASS_FORWARD_DECLARATION(RigidBody2D);
CLASS_FORWARD_DECLARATION(Body2D);
CLASS_FORWARD_DECLARATION(PhysicScriptBase);
CLASS_FORWARD_DECLARATION(Collider2DBase);
CLASS_FORWARD_DECLARATION(GameObj);

class PhysicComponent
{
public:
	class Component2D
	{
	public:
		Component2D(GameObj* owner);

		void ChangeBody(RigidBody2D* script);
		void RemoveBody(RigidBody2D* script);

		void CallOnChange();
		void Subscribe(Collider2DBase* collider);
		void UnSubscribe(Collider2DBase* collider);

		[[nodiscard]] RigidBody2D* GetRigidBodyScript() const noexcept;
		[[nodiscard]] WBody2D GetBody2D() const noexcept;
	protected:
		GameObj* _owner = nullptr;
		SBody2D _body;
		RigidBody2D* _bodyScript = nullptr;
		std::set<Collider2DBase*> _colliders;
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

	void Set2DBody		(RigidBody2D* script);
	void Remove2DBody	(RigidBody2D* script);

	[[nodiscard]] WBody2D GetBody2D() const noexcept;
	[[nodiscard]] RigidBody2D* GetRigidBody2DScript() const noexcept;

	PhysicComponent(GameObj* owner);
protected:
	std::set<PhysicScriptBase*> _physicComponents;

	mutable Component2D _2DComponents;
	mutable Component3D _3DComponents;
	GameObj* _owner;
};