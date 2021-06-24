#include "Collision.h"
#include "Circle.h"
#include "Polygon.h"
#include "SMath.h"
#include <cmath>

std::vector<std::vector<bool (*)(Collision*)>> Collision::_collisionFunctions = {
	{&Circle::CircleCircle, &Circle::CirclePolygon,},
	{&Polygon::PolygonCircle, &Polygon::PolygonPolygon}
};

CollideEvent Collision::_shapeACollideTemplate;
CollideEvent Collision::_shapeBCollideTemplate;

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

	// Broad phase
	float radiusSum = _shapeA->GetRadius() + _shapeB->GetRadius();
	Vector3 vec = _shapeA->GetCenter() - _shapeB->GetCenter();
	float distance = vec.GetPow2Magnitude();
	if (radiusSum * radiusSum < distance)
		return false;

	bool isCollide = (_collisionFunctions[static_cast<unsigned>(_shapeA->GetType())][static_cast<unsigned>(_shapeB->GetType())])(this);
	if (isCollide)
	{
		_shapeACollideTemplate.Reset(_shapeB->GetBody());
		_shapeBCollideTemplate.Reset(_shapeA->GetBody());
		_shapeA->SendEvent(_shapeACollideTemplate);
		_shapeB->SendEvent(_shapeBCollideTemplate);

		if (_shapeACollideTemplate.GetIsHandled() || _shapeBCollideTemplate.GetIsHandled())
			return false;
	}
	return isCollide;
}

void Collision::Initialize()
{
	SMaterial matA = _bodyA->GetMaterial().lock();
	SMaterial matB = _bodyB->GetMaterial().lock();

	_restitution = std::sqrt(matA->GetRestitution() * matA->GetRestitution() + matB->GetRestitution() * matB->GetRestitution());
	_staticFriction = std::sqrt(matA->GetStaticFriction() * matA->GetStaticFriction() + matB->GetStaticFriction() * matB->GetStaticFriction());
	_dynamicFriction = std::sqrt(matA->GetDynamicFriction() * matA->GetDynamicFriction() + matB->GetDynamicFriction() * matB->GetDynamicFriction());

	Vector3 relativeVel = _bodyB->GetVelocity() - _bodyA->GetVelocity();
}

void Collision::ApplyImpulse()
{
	if (SMath::compare(_bodyA->GetInverseMass() + _bodyB->GetInverseMass(), 0.0f))
	{
		InfiniteMassCorrection();
		return;
	}

	Vector3 relativeVel = _bodyB->GetVelocity() - _bodyA->GetVelocity();
	float velAlongNormal = relativeVel.Dot(_normal);

	if (velAlongNormal > 0)
		return;

	float inverseMassSum = _bodyA->GetInverseMass() + _bodyB->GetInverseMass();
	float j = -(1.0f + _restitution) * velAlongNormal;
	j /= inverseMassSum;

	Vector3 impulse = _normal * j;
	_bodyA->ApplyImpulse(-impulse);
	_bodyB->ApplyImpulse(impulse);

	Vector3 friction = relativeVel - (_normal * (relativeVel.Dot(_normal)));
	friction = friction.GetUnitVector();
	
	float frictionForce = -relativeVel.Dot(friction);
	frictionForce /= inverseMassSum;
	
	if (SMath::CompareFloat(frictionForce, 0.0f, 0.000001f))
		return;
	
	Vector3 tangentImpulse;
	if (std::abs(frictionForce) < j * _staticFriction)
		tangentImpulse = friction * frictionForce;
	else
		tangentImpulse = friction * (-j) * _dynamicFriction;

	_bodyA->ApplyImpulse(-tangentImpulse);
	_bodyB->ApplyImpulse(tangentImpulse);
}

void Collision::PositionalCorrection()
{
	float sumInverseMass = _shapeA->GetInverseMass() + _shapeB->GetInverseMass();
	if (SMath::CompareFloat(sumInverseMass, 0.0f, 0.0000001f))
		return;

	constexpr float kSlop = 0.05f;
	constexpr float percent = 0.8f;
	float correctionFloat = percent * (SMath::Max(_penetration - kSlop, 0.0f) / sumInverseMass);
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
