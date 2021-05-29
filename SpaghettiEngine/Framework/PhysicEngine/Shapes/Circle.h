#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	Type GetType() const override;
	[[nodiscard]] Vector3 GetCenter() const;

	static bool CircleCircle(Collision* collision);
protected:
	float _radius = 0;
};