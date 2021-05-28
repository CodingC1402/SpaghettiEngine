#pragma once
#include "Body2D.h"

class Shape;
typedef std::weak_ptr<Shape> WShape;
typedef std::shared_ptr<Shape> SShape;

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
	virtual void ComputeMass(float density) = 0;
	virtual void SetOrient(float radians) = 0;
	virtual void Draw(void) const = 0;
	virtual Type GetType(void) const = 0;

	WBody2D GetBody();
protected:
	WBody2D _body = Body2D::GetDefaultBody();
};