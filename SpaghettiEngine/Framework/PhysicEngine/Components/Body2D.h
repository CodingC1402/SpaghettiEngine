#pragma once
#include <memory>
#include "Vector3.h"
#include "Material.h"
#include <list>

class Body2D;
typedef std::weak_ptr<Body2D> WBody2D;
typedef std::shared_ptr<Body2D> SBody2D;

class Shape;
typedef std::weak_ptr<Shape> WShape;
typedef std::shared_ptr<Shape> SShape;

class Body2D 
{
public:
	static WBody2D GetDefaultBody();

	void AddShape(const SShape& shape);
	void RemoveShape(const SShape& shape);

	void SetOrient(float radians);
	void SetStatic();
	void ApplyImpulse(const Vector3& impulse, const Vector3& contactVector);
	void ApplyForce(const Vector3& force);
protected:
	static SBody2D _defaultBody;

	Vector3 _position;
	Vector3 _velocity;

	Vector3 _force;

	float _inertia = 0;
	float _inverseInertia = 0;

	float _mass = 0;
	float _inverseMass = 0;

	float _orient = 0;
	float _torque = 0;
	float _angularVelocity = 0;

	std::list<SShape> _shapes;
	WMaterial _material;
};