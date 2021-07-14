#pragma once
#include "Shape.h"
#include <vector>

class Collision
{
	friend class Physic;
public:
	Collision(Shape* A, Shape* B);

	[[nodiscard]] Shape* GetShapeA() const noexcept;
	[[nodiscard]] Shape* GetShapeB() const noexcept;

	[[nodiscard]] float GetRestituation() const noexcept;
	[[nodiscard]] float GetDynamicFriction() const noexcept;
	[[nodiscard]] float GetStaticFriction() const noexcept;

	void SetNormal(const Vector3& vec) noexcept;
	void SetPenetration(const float& pen) noexcept;

	void SetRestituation(const float& f) noexcept;
	void SetDynamicFriction(const float& f) noexcept;
	void SetStaticFriction(const float& f) noexcept;

	bool Solve(bool sendEvent);
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

	static CollideEvent _shapeACollideTemplate;
	static CollideEvent _shapeBCollideTemplate;
	static std::vector<std::vector<bool (*)(Collision*)>> _collisionFunctions;
};