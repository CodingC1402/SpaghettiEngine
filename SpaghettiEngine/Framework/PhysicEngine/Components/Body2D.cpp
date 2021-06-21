#include "Body2D.h"
#include "Shape.h"
#include "SMath.h"
#include "Physic.h"
#include "Collider2DBase.h"

SBody2D Body2D::_defaultBody = std::make_shared<Body2D>();

WBody2D Body2D::GetDefaultBody()
{
	return _defaultBody;
}

void Body2D::AddShape(Shape* shape)
{
	for (auto it = _shapes.begin(); it != _shapes.end(); ++it)
		if (*it == shape)
			return;
	_shapes.emplace_back(shape);
}

void Body2D::RemoveShape(Shape* shape)
{
	for (auto it = _shapes.begin(); it != _shapes.end(); ++it)
		if (*it == shape)
		{
			_shapes.erase(it);
			return;
		}
}

void Body2D::SetMass(const float& mass)
{
	if (mass == 0)
		_inverseMass = 0;
	else
		_inverseMass = 1 / mass;
	_mass = mass;
}

const float& Body2D::GetMass() const
{
	return _mass;
}

const float& Body2D::GetInverseMass() const
{
	return _inverseMass;
}

void Body2D::SetVelocity(const Vector3& velocity)
{
	_velocity = velocity;
}

const Vector3& Body2D::GetVelocity() const
{
	return _velocity;
}

void Body2D::SetForce(const Vector3& force)
{
	_force = force;
}

const Vector3& Body2D::GetForce() const
{
	return _force;
}

void Body2D::SetPosition(const Vector3& pos)
{
	_worldMatrix._41 = pos.x;
	_worldMatrix._42 = pos.y;
	OnUpdateMatrix();
}

const Vector3 Body2D::GetPosition() const
{
	return Vector3(_worldMatrix._41, _worldMatrix._42, 0);
}

void Body2D::SetWorldMatrix(const Matrix4& mat)
{
	_worldMatrix = mat;
	OnUpdateMatrix();
}

const Matrix4& Body2D::GetWorldMatrix() const
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

const float& Body2D::GetGravityScale() const
{
	return _gravityScale;
}

void Body2D::SetGameObject(GameObj* collider)
{
	_gameObject = collider;
}

GameObj* Body2D::GetGameObject() const
{
	return _gameObject;
}

void Body2D::SendEvent(CollideEvent& e)
{
	_gameObject->OnCollide(e);
	_currentCollide.emplace_back(e.GetBody());
}

void Body2D::SendExitEnterEvent()
{
	std::list<WBody2D> newCurrentCollide;
	bool isNewCollide = false;

	/// <summary>
	/// Loop and remove the repeated collide and add the new collide to newCurrentCollide
	/// </summary>
	for (auto newIt = _currentCollide.begin(); newIt != _currentCollide.end(); ++newIt)
	{
		isNewCollide = true;

		for (auto oldIt = _collidedBody.begin(); oldIt != _collidedBody.end(); ++oldIt)
		{
			if ((*newIt).lock() == (*oldIt).lock())
			{
				_collidedBody.erase(oldIt);
				isNewCollide = false;
				break;
			}
		}

		if (isNewCollide)
			newCurrentCollide.emplace_back(*newIt);
	}

	for (const auto& oldBody : _collidedBody)
	{
		CollideEvent newEvent(oldBody);
		_gameObject->OnCollideExit(newEvent);
	}

	for (const auto& newBody : newCurrentCollide)
	{
		CollideEvent newEvent(newBody);
		_gameObject->OnCollideEnter(newEvent);
	}

	_collidedBody = _currentCollide;
	newCurrentCollide.clear();
	_currentCollide.clear();
}

Vector3 Body2D::GetMoveVector()
{
	return Vector3();
}

float Body2D::GetRotation()
{
	return _rotation;
}

void Body2D::SetMaterial(WMaterial material)
{
	_material = material.lock();
}

WMaterial Body2D::GetMaterial() const
{
	return _material;
}

Body2D* Body2D::Clone() const
{
	auto cloneBody = new Body2D();

	cloneBody->_velocity	= _velocity;
	cloneBody->_force		= _force;
	cloneBody->_rotation	= _rotation;
	cloneBody->_mass		= _mass;
	cloneBody->_inverseMass = _inverseMass;
	cloneBody->_gravityScale = _gravityScale;
	cloneBody->_moveVec		= _moveVec;
	cloneBody->_material	= _material;

	for (const auto& shape : _shapes)
		cloneBody->_shapes.push_back(shape->Clone());

	return cloneBody;
}

void Body2D::SetMaterialToDefault()
{
	_material = Material::GetDefaultMaterial().lock();
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
	_moveVec = _velocity * Physic::GetStep();
	SMath::TranslateMatrix(_worldMatrix, _moveVec);
	OnUpdateMatrix();
}

void Body2D::OnUpdateMatrix()
{
	for (auto& shape : _shapes)
		shape->UpdateParameter();
}
