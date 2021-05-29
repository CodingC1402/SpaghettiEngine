#pragma once
#include "Shape.h"
#include <vector>

class Collision
{
	friend class Physic;
public:
	Collision(WShape A, WShape B);

	[[nodiscard]] WShape GetShapeA();
	[[nodiscard]] WShape GetShapeB();

	[[nodiscard]] float GetRestituation();
	[[nodiscard]] float GetDynamicFriction();
	[[nodiscard]] float GetStaticFriction();

	void SetNormal(const Vector3& vec);
	void SetPenetration(const float& pen);

	void SetRestituation(const float& f);
	void SetDynamicFriction(const float& f);
	void SetStaticFriction(const float& f);

	bool Solve();
	void Initialize();            // Recalculations for impulse solving
	void ApplyImpulse();          // Solve impulse and apply
	void PositionalCorrection();  // Naive correction of positional penetration
	void InfiniteMassCorrection();
protected:
	WShape _shapeA;
	WShape _shapeB;

	float _penetration;
	Vector3 _normal;

	float _restitution;
	float _dynamicFriction;
	float _staticFriction;

	static std::vector<std::vector<bool (*)(Collision*)>> _collisionFunctions;
};