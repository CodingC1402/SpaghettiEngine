#include "Collision.h"
#include "Circle.h"
#include "Polygon.h"
#include "SMath.h"
#include "Collider2DBase.h"

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

Shape* Collision::GetShapeA() const noexcept
{
	return _shapeA;
}

Shape* Collision::GetShapeB() const noexcept
{
	return _shapeB;
}



float Collision::GetRestituation() const noexcept
{
	return _restitution;
}

float Collision::GetDynamicFriction() const noexcept
{
	return _dynamicFriction;
}

float Collision::GetStaticFriction() const noexcept
{
	return _staticFriction;
}

void Collision::SetNormal(const Vector3& vec) noexcept
{
	_normal = vec;
}

void Collision::SetPenetration(const float& pen) noexcept
{
	_penetration = pen;
}

void Collision::SetRestituation(const float& f) noexcept
{
	_restitution = f;
}

void Collision::SetDynamicFriction(const float& f) noexcept
{
	_dynamicFriction = f;
}

void Collision::SetStaticFriction(const float& f) noexcept
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
		// The normal is the unit vector that connect point from shapeA to shapeB
		// so you have to * -1 on message for shapeB to reverse the unit vector to
		// point from shapeB to shapeA.
		// Remember that future me :D ? Or who ever copying this code ~~ god damn do it your self.
		_shapeACollideTemplate.Reset(_shapeA->GetOwnerScript(), _shapeB->GetBody().lock().get(), _shapeB->GetOwnerScript(), _normal);
		_shapeBCollideTemplate.Reset(_shapeB->GetOwnerScript(), _shapeA->GetBody().lock().get(), _shapeA->GetOwnerScript(), _normal * -1);

		if (_shapeA->IsTriggerOnly())
			_shapeACollideTemplate.SetToTrigger();
		if (_shapeB->IsTriggerOnly())
			_shapeBCollideTemplate.SetToTrigger();

		_shapeA->SendEvent(_shapeACollideTemplate);			    
		_shapeB->SendEvent(_shapeBCollideTemplate);

		bool ignoreCollision =	_shapeACollideTemplate.GetIsHandled() | _shapeACollideTemplate.IsCollideWithTrigger() | 
								_shapeBCollideTemplate.GetIsHandled() | _shapeBCollideTemplate.IsCollideWithTrigger();

		if (ignoreCollision)
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
