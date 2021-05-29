#include "Circle.h"
#include "Collision.h"

Shape::Type Circle::GetType() const
{
	return Shape::Type::Circle;
}

void Circle::AddBody(WBody2D newBody)
{
	_body = newBody;
}

void Circle::RemoveBody()
{
	_body = Body2D::GetDefaultBody();
}

void Circle::UpdateTransform(const Matrix4& matrix)
{
	_position = _position * matrix;
}

bool Circle::CircleCircle(Collision* collision)
{
	auto shapeA = std::dynamic_pointer_cast<Circle>(collision->GetShapeA().lock());
	auto shapeB = std::dynamic_pointer_cast<Circle>(collision->GetShapeB().lock());

	Vector3 = shapeA->_position - shapeB->_position;
}
