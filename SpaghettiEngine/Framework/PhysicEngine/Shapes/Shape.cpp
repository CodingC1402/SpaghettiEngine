#include "Shape.h"
#include "Physic.h"

WBody2D Shape::GetBody()
{
	return _body;
}

Shape::Shape()
{
	_offSetMatrix = Matrix4::GetDiagonalMatrix();
}

float Shape::GetRestitution()
{
	return _body.lock()->_material.lock()->_restitution;
}

float Shape::GetStaticFriction()
{
	return _body.lock()->_material.lock()->_staticFriction;
}

float Shape::GetDynamicFriction()
{
	return _body.lock()->_material.lock()->_dynamicFriction;
}

Vector3 Shape::GetGravityVector()
{
	return Physic::GetGravity() * _body.lock()->_gravityScale;
}

float Shape::GetInverseMass()
{
	return _body.lock()->_inverseMass;
}

float Shape::GetMass()
{
	return _body.lock()->_mass;
}

Vector3 Shape::GetVelocity()
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

const float& Shape::GetOffSetX()
{
	return _offSetMatrix._41;
}

const float& Shape::GetOffSetY()
{
	return _offSetMatrix._42;
}

void Shape::AddToPhysic()
{
}

void Shape::SetBody(WBody2D body)
{
	if (!_isStatic)
		return;
	_body = body;
	_isStatic = false;
}

void Shape::ToStaic()
{
	if (_isStatic)
		return;
	_body = Body2D::GetDefaultBody();
	_isStatic = true;
}
