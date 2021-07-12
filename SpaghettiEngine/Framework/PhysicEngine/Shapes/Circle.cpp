#include "Circle.h"
#include "Collision.h"
#include "Polygon.h"

Shape::Type Circle::GetType() const
{
	return Shape::Type::Circle;
}

void Circle::SetRadius(const float& radius)
{
	_radius = radius;
}

Shape* Circle::Clone() const
{
	Circle* cloneCircle = dynamic_cast<Circle*>(Shape::Clone());
	
	cloneCircle->_radius = _radius;

	return cloneCircle;
}

bool Circle::CircleCircle(Collision* collision)
{
	auto shapeA = dynamic_cast<Circle*>(collision->GetShapeA());
	auto shapeB = dynamic_cast<Circle*>(collision->GetShapeB());

	Vector3 normal = shapeB->GetCenter() - shapeA->GetCenter();
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

const float& Circle::GetRadius() const
{
	return _radius;
}

bool Circle::CirclePolygon(Collision* collision)
{
	const auto polygon = dynamic_cast<Polygon2D*>(collision->GetShapeB());
	const auto circle = dynamic_cast<Circle*>(collision->GetShapeA());

	Vector3 normal;
	float penetration;

	if (!Polygon::PolygonCircleCollision(*polygon, *circle, penetration, normal))
		return false;

	if (normal.Dot(collision->GetShapeB()->GetCenter() - collision->GetShapeA()->GetCenter()) < 0)
		normal = -normal;

	collision->SetPenetration(penetration);
	collision->SetNormal(normal);

	return true;
}
