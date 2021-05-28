#pragma once
#include "Shape.h"

class Collision
{
public:
	Collision(WShape A, WShape B);

	bool Solve();
public:
	WShape _shapeA;
	WShape _shapeB;

	float _penetration;
	Vector3 _normal;

	float _restitution;
	float _dynamicFriction;
	float _staticFriction;

	bool (*_dispatcher)(Collision*, WShape, WShape);
};