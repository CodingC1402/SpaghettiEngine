#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	Type GetType() const override;
	void AddBody(WBody2D newBody);
	void RemoveBody();
	void UpdateTransform(const Matrix4& matrix) override;

	static bool CircleCircle(Collision* collision);
protected:
	float radius = 0;
};