#include "PhysicCollide.h"
#include "Circle.h"
#include "Polygon.h"

void PhysicCollide::GetCollidedWithRectangle(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	const Vector3& center, 
	const float& width, 
	const float& height,
	const Tag& filter,
	const FilterMode& mode)
{
	Vector3 topLeft, topRight, bottomLeft, bottomRight;

	bottomLeft.x = center.x - width / 2.0f;
	bottomLeft.y = center.y - height / 2.0f;

	topLeft.x = bottomLeft.x;
	topLeft.y = bottomLeft.y + height;

	bottomRight.y = bottomLeft.y;
	bottomRight.x = bottomLeft.x + width;

	topRight.x = bottomRight.x;
	topRight.y = topLeft.y;

	GetCollidedWithPolygon(gameObject, gameObjectList, {bottomLeft, topLeft, topRight, bottomRight}, filter, mode);
}

void PhysicCollide::GetCollidedWithPolygon(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	const std::vector<Vector3>& vertexes,
	const Tag& filter,
	const FilterMode& mode)
{
	Polygon2D polygon;
	polygon.SetVertexes(vertexes);
	GetCollidedWithShape(gameObject, gameObjectList, &polygon, filter, mode);
}

void PhysicCollide::GetCollidedWithCircle(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	const Vector3& center, 
	const float& radius,
	const Tag& filter,
	const FilterMode& mode)
{
	Circle circle;
	circle.SetRadius(radius);

	circle.SetOffSetX(center.x);
	circle.SetOffSetY(center.y);

	GetCollidedWithShape(gameObject, gameObjectList, &circle, filter, mode);
}

void PhysicCollide::GetCollidedWithShape(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	Shape* shape,
	const Tag& filter,
	const FilterMode& mode)
{
	std::list<Collision> collisionList;
	// Update to current matrix
	shape->UpdateParameter(gameObject->GetTransform().GetWorldMatrix());
	// Use quad tree to create collide list
	Physic::GetQuadTree().CreateCollisionListWithShape(collisionList, shape);

	auto it = collisionList.begin();
	Shape* collideShape;
	GameObj* gameObjectCollideWith;
	bool tagResult = false;

	while (it != collisionList.end())
	{
		collideShape = (*it).GetShapeA() == shape ? (*it).GetShapeB() : (*it).GetShapeA();
		gameObjectCollideWith = collideShape->GetBody().lock()->GetGameObject();

		tagResult = false;

		switch (mode)
		{
		case FilterMode::Collide:
			tagResult = gameObjectCollideWith->GetTag().Collide(filter);
			break;
		case FilterMode::Contain:
			tagResult = gameObjectCollideWith->GetTag().Contain(filter);
			break;
		case FilterMode::Equal:
			tagResult = gameObjectCollideWith->GetTag() == filter;
		case FilterMode::None:
			tagResult = true;
			break;
		}

		if (tagResult && gameObjectList.find(gameObjectCollideWith) == gameObjectList.end())
		{
			if ((*it).Solve(false))
			{
				gameObjectList.emplace(gameObjectCollideWith);
			}
		}

		it = collisionList.erase(it);
	}
}
