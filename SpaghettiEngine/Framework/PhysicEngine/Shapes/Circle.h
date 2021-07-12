#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	Type GetType() const override;
	void SetRadius(const float& radius);

	Shape* Clone() const override;

	[[nodiscard]] const float& GetRadius() const;

	static bool CircleCircle(Collision* collision);
	static bool CirclePolygon(Collision* collision);
protected:
	//Use for broad phase
	float _radius;
};