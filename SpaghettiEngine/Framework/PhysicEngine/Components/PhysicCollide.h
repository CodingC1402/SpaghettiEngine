#pragma once
#include "Physic.h"
#include "Tag.h"
#include <set>

class PhysicCollide
{
public:
	enum class FilterMode
	{
		Contain,
		Collide,
		Equal,
		None
	};
public:
	static void GetCollidedWithRectangle(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList, 
		const Vector3& center, 
		const float& width, 
		const float& height,
		const Tag& filter = Tag(),
		const FilterMode& mode = FilterMode::None
	);

	static void GetCollidedWithPolygon(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList,
		const std::vector<Vector3>& vertexes,
		const Tag& filter = Tag(),
		const FilterMode& mode = FilterMode::None
	);

	static void GetCollidedWithCircle(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList, 
		const Vector3& center, 
		const float& radius,
		const Tag& filter = Tag(),
		const FilterMode& mode = FilterMode::None
	);

	static void GetCollidedWithShape(
		GameObj* gameObject,
		std::set<GameObj*>& gameObjectList, 
		Shape* shape,
		const Tag& filter = Tag(),
		const FilterMode& mode = FilterMode::None
	);
};