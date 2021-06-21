#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	Type GetType() const override;
	void SetRadius(const float& radius);

	Shape* Clone() const override;
	void UpdateParameter() override;

	static bool CircleCircle(Collision* collision);
	static bool CirclePolygon(Collision* collision);
};