#include "Collision.h"
#include "Circle.h"
#include "Polygon.h"
#include "SMath.h"

std::vector<std::vector<bool (*)(Collision*)>> Collision::_collisionFunctions = {
	{&Circle::CircleCircle, &Polygon::CirclePolygon,},
	{&Polygon::PolygonCircle,& Polygon::PolygonPolygon}
};

Collision::Collision(Shape* A, Shape* B)
{
	_shapeA = A;
	_shapeB = B;
}

Shape* Collision::GetShapeA()
{
	return _shapeA;
}

Shape* Collision::GetShapeB()
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

void Collision::SetNormal(const Vector3& vec)
{
	_normal = vec;
}

void Collision::SetPenetration(const float& pen)
{
	_penetration = pen;
}

void Collision::SetRestituation(const float& f)
{
	_restitution = f;
}

void Collision::SetDynamicFriction(const float& f)
{
	_dynamicFriction = f;
}

void Collision::SetStaticFriction(const float& f)
{
	_staticFriction = f;
}


bool Collision::Solve()
{
	return (_collisionFunctions[static_cast<unsigned>(_shapeA->GetType())][static_cast<unsigned>(_shapeB->GetType())])(this);
}

void Collision::Initialize()
{

}

void Collision::PositionalCorrection()
{
	constexpr float kSlop = 0.05f;
	constexpr float percent = 0.4f;
	float correctionFloat = percent * (SMath::Max(_penetration - kSlop, 0.0f) / (_shapeA->GetInverseMass() + _shapeB->GetInverseMass()));
	Vector3 correction(_normal * correctionFloat);
}
