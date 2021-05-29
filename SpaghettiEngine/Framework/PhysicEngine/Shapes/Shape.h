#pragma once
#include "Body2D.h"
#include "Matrix.h"

class Shape;
typedef std::weak_ptr<Shape> WShape;
typedef std::shared_ptr<Shape> SShape;

class Collision;

class Shape
{
public:
	enum class Type
	{
		Circle,
		Polygon,
		TypeCount
	};
public:
	virtual void Initialize() = 0;
	virtual void UpdateTransform(const Matrix4& matrix) = 0;
	virtual Type GetType() const = 0;

	[[nodiscard]] float GetRestitution();
	[[nodiscard]] float GetStaticFriction();
	[[nodiscard]] float GetDynamicFriction();
	[[nodiscard]] float GetGravity();
	[[nodiscard]] Vector3 GetPosition();

	WBody2D GetBody();
protected:
	WBody2D _body = Body2D::GetDefaultBody();
	Vector3 _position;
};