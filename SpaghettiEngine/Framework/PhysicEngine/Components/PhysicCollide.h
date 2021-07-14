#pragma once
#include "Physic.h"
#include <set>

class PhysicCollide
{
public:
	static void GetCollidedWithRectangle(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList, 
		const Vector3& bottomLeft, 
		const float& width, 
		const float& height);

	static void GetCollidedWithPolygon(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList,
		const std::vector<Vector3>& vertexes);

	static void GetCollidedWithCircle(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList, 
		const Vector3& center, 
		const float& radius);

	static void GetCollidedWithShape(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList, 
		Shape* shape);
};