#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	Type GetType() const override;
	[[nodiscard]] Vector3 GetCenter() const;

	void SetRadius(const float& radius);
	[[nodiscard]] float& GetRadius();

	static bool CircleCircle(Collision* collision);
protected:
	float _radius = 1.0f;
};