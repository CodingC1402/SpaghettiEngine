#pragma once
#include "Shape.h"

class Collision;

class Polygon : public Shape
{
public:
	static bool PolygonPolygon(Collision* collision, WShape shapeA, WShape shapeB);
	static bool CirclePolygon(Collision* collision, WShape shapeA, WShape shapeB);
	static bool PolygonCircle(Collision* collision, WShape shapeA, WShape shapeB);
};