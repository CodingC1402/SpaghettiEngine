#include "Collision.h"
#include "Circle.h"
#include "Polygon.h"

std::vector<std::vector<bool (*)(Collision*)>> Collision::_collisionFunctions = {
	{&Circle::CircleCircle, &Polygon::CirclePolygon,},
	{&Polygon::PolygonCircle,& Polygon::PolygonPolygon}
};

Collision::Collision(WShape A, WShape B)
{
	_shapeA = A;
	_shapeB = B;
}

WShape Collision::GetShapeA()
{
	return _shapeA;
}

WShape Collision::GetShapeB()
{
	return _shapeB;
}

float Collision::GetRestituation()
{
	return _restitution;
}

float Collision::GetDynamicFriction()
{
	return _dynamicFriction;
}

float Collision::GetStaticFriction()
{
	return _staticFriction;
}

bool Collision::Solve()
{
	return (_collisionFunctions[static_cast<unsigned>(_shapeA.lock()->GetType())][static_cast<unsigned>(_shapeB.lock()->GetType())])(this);
}

void Collision::Initialize()
{

}
