#include "Shape.h"
#include "Body2D.h"
#include "Physic.h"
#include "Circle.h"
#include "Polygon.h"

constexpr auto circle = "Circle";
constexpr auto polygon = "Polygon";

std::map<const char*, Shape*(*)()> ShapeFactory::_functions
{
	{circle, &ShapeFactory::CreateNew<Circle> },
	{polygon, &ShapeFactory::CreateNew<Polygon2D>}
};

Shape* ShapeFactory::Create(const Shape::Type& type)
{
	std::string typeStr;
	switch (type)
	{
	case Shape::Type::Circle:
		typeStr = circle;
		break;
	case Shape::Type::Polygon:
		typeStr = polygon;
		break;
	}
	return Create(typeStr);
}

Shape* Shape::Clone() const
{
	auto cloneShape = ShapeFactory::Create(GetType());

	cloneShape->_broadPhase = _broadPhase;
	cloneShape->_offSetMatrix = _offSetMatrix;

	return cloneShape;
}

WBody2D Shape::GetBody() const noexcept
{
	return _body;
}

Shape::Shape()
{
	_offSetMatrix = Matrix4::GetDiagonalMatrix();
}

const Vector3& Shape::GetCenter() const noexcept
{
	return _broadPhase.GetWorldCenter();
}

const BroadPhaseShape& Shape::GetBroadPhase() const noexcept
{
	return _broadPhase;
}

Shape::~Shape()
{
	Physic::RemoveShape(this);
}

Shape::Type Shape::GetType() const
{
	return Shape::Type::Invalid;
}

Vector3 Shape::GetGravityVector() const noexcept
{
	return Physic::GetGravity() * _body.lock()->_gravityScale;
}

void Shape::SetOwnerScript(Collider2DBase* owner)
{
	_ownerScript = owner;
}

Collider2DBase* Shape::GetOwnerScript() const noexcept
{
	return  _ownerScript;
}

float Shape::GetInverseMass() const noexcept
{
	return _body.lock()->_inverseMass;
}

float Shape::GetMass() const noexcept
{
	return _body.lock()->_mass;
}

Vector3 Shape::GetVelocity() const noexcept
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

float Shape::GetOffSetX() const noexcept
{
	return _offSetMatrix._41;
}

float Shape::GetOffSetY() const noexcept
{
	return _offSetMatrix._42;
}

bool Shape::IsTriggerOnly() const noexcept
{
	return _isTriggerOnly;
}

void Shape::SetTriggerOnly(bool value)
{
	_isTriggerOnly = value;
}

void Shape::SendEvent(CollideEvent& e)
{
	_body.lock()->SendEvent(e);
}

bool Shape::UpdateParameter()
{
	Matrix4 matrix = _body.lock()->GetWorldMatrix();
	if (matrix == _worldMatrix)
		return false;

	_worldMatrix = matrix;
	_broadPhase.UpdateMatrix(_worldMatrix, _offSetMatrix);
	return true;
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

Shape* ShapeFactory::Create(const std::string& type)
{
	return _functions[type.c_str()]();
}
