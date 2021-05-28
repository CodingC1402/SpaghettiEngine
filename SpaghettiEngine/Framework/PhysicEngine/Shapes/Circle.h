#pragma once
#include "Shape.h"

class Collision;

class Circle : public Shape
{
public:
	static bool CircleCircle(Collision* collision, WShape shapeA, WShape shapeB);
};