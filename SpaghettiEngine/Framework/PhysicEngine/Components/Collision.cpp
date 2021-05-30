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
	_bodyA = A->GetBody().lock().get();
	_bodyB = B->GetBody().lock().get();
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
	if (_bodyA == _bodyB)
		return false;
	return (_collisionFunctions[static_cast<unsigned>(_shapeA->GetType())][static_cast<unsigned>(_shapeB->GetType())])(this);
}

void Collision::Initialize()
{
	_restituation = 0;

	_staticFriction = 2;
	_dynamicFriction = 1;
}

void Collision::ApplyImpulse()
{
	if (SMath::compare(_bodyA->GetInverseMass() + _bodyB->GetInverseMass(), 0.0f))
	{
		InfiniteMassCorrection();
		return;
	}

	Vector3 rv = _bodyB->GetVelocity() - _bodyA->GetVelocity();
	float velAlongNormal = rv.Dot(_normal);

	if (velAlongNormal > 0)
		return;

	float inverseMassSum = _bodyA->GetInverseMass() + _bodyB->GetInverseMass();
	float j = -1 * velAlongNormal;
	j /= inverseMassSum;

	Vector3 impulse = _normal * j;
	_bodyA->ApplyImpulse(-impulse);
	_bodyB->ApplyImpulse(impulse);
}

void Collision::PositionalCorrection()
{
	constexpr float kSlop = 0.05f;
	constexpr float percent = 0.8f;
	float correctionFloat = percent * (SMath::Max(_penetration - kSlop, 0.0f) / (_shapeA->GetInverseMass() + _shapeB->GetInverseMass()));
	Vector3 correction(_normal * correctionFloat);

	auto bodyA = _shapeA->GetBody().lock();
	auto bodyB = _shapeB->GetBody().lock();
	bodyA->SetPosition(bodyA->GetPosition() - correction * _shapeA->GetInverseMass());
	bodyB->SetPosition(bodyB->GetPosition() + correction * _shapeB->GetInverseMass());
}

void Collision::InfiniteMassCorrection()
{
	auto bodyA = _shapeA->GetBody().lock();
	auto bodyB = _shapeB->GetBody().lock();
	bodyA->SetVelocity(Vector3(0, 0, 0));
	bodyB->SetVelocity(Vector3(0, 0, 0));
}
