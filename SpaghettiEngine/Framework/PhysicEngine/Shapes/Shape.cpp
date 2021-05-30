#include "Shape.h"
#include "Physic.h"

WBody2D Shape::GetBody()
{
	return _body;
}

Shape::Shape()
{
	_offSetMatrix = Matrix4::GetDiagonalMatrix();
	_staticBody = std::make_shared<Body2D>();
	_body = _staticBody;
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
	if (!_isStatic)
		return;
	_body.lock()->RemoveShape(this);
	_body = body;
	_isStatic = false;
	_staticBody.reset();
	_body.lock()->AddShape(this);
}

void Shape::ToStatic()
{
	if (_isStatic)
		return;
	_body.lock()->RemoveShape(this);
	_staticBody = std::make_shared<Body2D>();
	_staticBody->SetWorldMatrix(_body.lock()->GetWorldMatrix());
	_body = _staticBody;
	_isStatic = true;
}
