#include "Body2D.h"
#include "Shape.h"
#include "SMath.h"
#include "Physic.h"

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

void Body2D::SetPosition(const Vector3& pos)
{
	_worldMatrix._41 = pos.x;
	_worldMatrix._42 = pos.y;
}

const Vector3& Body2D::GetPosition()
{
	return Vector3(_worldMatrix._41, _worldMatrix._42, 0);
}

void Body2D::SetWorldMatrix(const Matrix4& mat)
{
	_worldMatrix = mat;
}

const Matrix4& Body2D::GetWorldMatrix()
{
	return _worldMatrix;
}

void Body2D::SetRotation(const float& degree)
{
	_rotation += degree;
	_worldMatrix *= SMath::GetZAxisRotateMatrix(degree);
}

const float& Body2D::GetRoation()
{
	return _rotation;
}

void Body2D::SetGravityScale(const float& scale)
{
	_gravityScale = scale;
}

const float& Body2D::GetGravityScale()
{
	return _gravityScale;
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

void Body2D::ApplyImpulse(const Vector3& impulse)
{
	_velocity += impulse * _inverseMass;
}

void Body2D::ApplyForce(const Vector3& force)
{
	_force += force;
}

void Body2D::IntergateForces()
{
	if (_inverseMass == 0.0f)
		return;
	_velocity += (_force * _inverseMass + Physic::GetGravity() * _gravityScale) * (Physic::GetStep());
}

void Body2D::IntergateVelocity()
{
	auto moveVec = _velocity * Physic::GetStep();
	SMath::TranslateMatrix(_worldMatrix, moveVec);
}
