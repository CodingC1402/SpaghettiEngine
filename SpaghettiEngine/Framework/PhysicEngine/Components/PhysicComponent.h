#pragma once
#include "Body2D.h"
#include "ObservePattern.h"

class Collider2DBase;

class PhysicComponent
{
public:
	class Component2D : public Observable
	{
	public:
		void ChangeBody(WBody2D body);
		void RemoveBody(WBody2D body);
		[[nodiscard]] WBody2D GetBody2D();
	protected:
		SBody2D _body;
	};

	class Component3D : public Observable
	{
	public:

	protected:
	};
public:
	void SubscribeTo2D(Collider2DBase* script);
	void UnSubscribeTo2D(Collider2DBase* script);

	void Set2DBody(WBody2D body);
	void Remove2DBody(WBody2D body);
	WBody2D GetBody2D();
protected:
	Component2D _2DComponents;
	Component3D _3DComponents;
};