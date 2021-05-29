#include "Circle.h"
#include "Collision.h"

Shape::Type Circle::GetType() const
{
	return Shape::Type::Circle;
}

Vector3 Circle::GetCenter() const
{
	return _body.lock()->GetPosition();
}

bool Circle::CircleCircle(Collision* collision)
{
	auto shapeA = std::dynamic_pointer_cast<Circle>(collision->GetShapeA().lock());
	auto shapeB = std::dynamic_pointer_cast<Circle>(collision->GetShapeB().lock());

	Vector3 normal = shapeA->GetCenter() - shapeB->GetCenter();
	float sumRadius = shapeA->_radius + shapeB->_radius;
	sumRadius *= sumRadius;

	if (normal.GetPow2Magnitude() > sumRadius)
		return false;

	sumRadius = shapeA->_radius + shapeB->_radius;
	float distance = normal.GetMagnitude();
	if (distance != 0)
	{
		collision->SetPenetration(sumRadius - distance);
		collision->SetNormal(normal / distance);
		return true;
	}
	else
	{
		collision->SetPenetration(shapeA->_radius);
		collision->SetNormal(Vector3(0, 1, 0));
		return true;
	}
}
