#include "Body2D.h"
#include "Shape.h"

SBody2D Body2D::_defaultBody = std::make_shared<Body2D>();

WBody2D Body2D::GetDefaultBody()
{
	return _defaultBody;
}

void Body2D::AddShape(const SShape& shape)
{
	if (shape->GetBody().lock().get() == this)
		return;
	_shapes.push_back(shape);
}

void Body2D::RemoveShape(const SShape& shape)
{
	_shapes.remove(shape);
}

void Body2D::SetMass(const float& mass)
{
	if (mass == 0)
		_inverseMass = 0;
	else
		_inverseMass = 1 / mass;
	_mass = mass;
}

const float& Body2D::GetMass()
{
	return _mass;
}

const float& Body2D::GetInverseMass()
{
	return _inverseMass;
}

void Body2D::SetVelocity(const Vector3& velocity)
{
	_velocity = velocity;
}

const Vector3& Body2D::GetVelocity()
{
	return _velocity;
}

void Body2D::SetPosity(const Vector3& pos)
{
	_position = pos;
}

const Vector3& Body2D::GetPosition()
{
	return _position;
}

void Body2D::Assign(WMaterial material)
{
	_material = material;
}

void Body2D::SetStatic()
{
	_mass = 0;
	_inverseMass = 0;
	_inertia = 0;
	_inverseInertia = 0;
	_force = Vector3();
}

void Body2D::ApplyImpulse(const Vector3& impulse, const Vector3& contactVector)
{
}

void Body2D::ApplyForce(const Vector3& force)
{
}
