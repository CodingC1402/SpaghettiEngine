#pragma once
#include "Shape.h"
#include <vector>

class Collision
{
	friend class Physic;
public:
	Collision(Shape* A, Shape* B);

	[[nodiscard]] Shape* GetShapeA();
	[[nodiscard]] Shape* GetShapeB();

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
	Shape* _shapeA = nullptr;
	Shape* _shapeB = nullptr;
	Body2D* _bodyA = nullptr;
	Body2D* _bodyB = nullptr;

	float _penetration = 0;
	Vector3 _normal;

	float _restitution = 0;
	float _dynamicFriction = 0;
	float _staticFriction = 0;

	float _restituation = 1.0f;

	static std::vector<std::vector<bool (*)(Collision*)>> _collisionFunctions;
};