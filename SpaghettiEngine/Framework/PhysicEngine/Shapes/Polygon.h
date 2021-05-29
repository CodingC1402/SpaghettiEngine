#pragma once
#include "Shape.h"

class Collision;

class Polygon : public Shape
{
public:
	static bool PolygonPolygon(Collision* collision);
	static bool CirclePolygon(Collision* collision);
	static bool PolygonCircle(Collision* collision);
};