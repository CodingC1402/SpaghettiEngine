#include "Shape.h"
#include "Body2D.h"
#include "Physic.h"

WBody2D Shape::GetBody() const
{
	return _body;
}

Shape::Shape()
{
	_offSetMatrix = Matrix4::GetDiagonalMatrix();
	_radius = 0;
}

Shape::~Shape()
{
	Physic::RemoveShape(this);
}

Shape::Type Shape::GetType() const
{
	return Shape::Type::Invalid;
}

Vector3 Shape::GetGravityVector() const
{
	return Physic::GetGravity() * _body.lock()->_gravityScale;
}

void Shape::SetOwnerScript(WCollider2DBase owner)
{
	_ownerScript = owner;
}

WCollider2DBase Shape::GetOwnerScript()
{
	return  _ownerScript;
}

float Shape::GetInverseMass() const
{
	return _body.lock()->_inverseMass;
}

float Shape::GetMass() const
{
	return _body.lock()->_mass;
}

Vector3 Shape::GetVelocity() const
{
	return _body.lock()->GetVelocity();
}

void Shape::SetOffSetX(const float& x)
{
	_offSetMatrix._41 = x;
}

void Shape::SetOffSetY(const float& y)
{
	_offSetMatrix._42 = y;
}

float Shape::GetOffSetX() const
{
	return _offSetMatrix._41;
}

float Shape::GetOffSetY() const
{
	return _offSetMatrix._42;
}

const Vector3& Shape::GetCenter() const
{
	return _center;
}

const float& Shape::GetRadius() const
{
	return _radius;
}

void Shape::SendEvent(CollideEvent& e)
{
	_body.lock()->SendEvent(e);
}

void Shape::RemoveFromPhysic()
{
	Physic::RemoveShape(this);
}

void Shape::AddToPhysic()
{
	Physic::AddShape(this);
}

void Shape::SetBody(WBody2D body)
{
	if (!_body.expired())
		_body.lock()->RemoveShape(this);
	_body = body;
	_isStatic = false;
	_body.lock()->AddShape(this);
}