#include "PhysicCollide.h"
#include "Circle.h"
#include "Polygon.h"

void PhysicCollide::GetCollidedWithRectangle(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	const Vector3& bottomLeft, 
	const float& width, 
	const float& height)
{
	Vector3 topLeft, bottomRight, topRight;

	topLeft.x = bottomLeft.x;
	topLeft.y = bottomLeft.y + height;

	bottomRight.y = bottomLeft.y;
	bottomRight.x = bottomLeft.x + width;

	topRight.x = bottomRight.x;
	topRight.y = topLeft.y;

	GetCollidedWithPolygon(gameObject, gameObjectList, {bottomLeft, topLeft, topRight, bottomRight});
}

void PhysicCollide::GetCollidedWithPolygon(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	const std::vector<Vector3>& vertexes)
{
	Polygon2D polygon;
	polygon.SetVertexes(vertexes);
	GetCollidedWithShape(gameObject, gameObjectList, &polygon);
}

void PhysicCollide::GetCollidedWithCircle(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	const Vector3& center, 
	const float& radius)
{
	Circle circle;
	circle.SetRadius(radius);

	circle.SetOffSetX(center.x);
	circle.SetOffSetY(center.y);

	GetCollidedWithShape(gameObject, gameObjectList, &circle);
}

void PhysicCollide::GetCollidedWithShape(
	GameObj* gameObject,
	std::set<GameObj*>& gameObjectList, 
	Shape* shape)
{
	std::list<Collision> collisionList;
	Physic::GetQuadTree().CreateCollisionListWithShape(collisionList, shape);
	shape->UpdateParameter(gameObject->GetTransform().GetWorldMatrix());

	auto it = collisionList.begin();
	Shape* collideShape;
	GameObj* gameObjectCollideWith;

	while (it != collisionList.end())
	{
		collideShape = (*it).GetShapeA() == shape ? (*it).GetShapeB() : (*it).GetShapeA();
		gameObjectCollideWith = collideShape->GetBody().lock()->GetGameObject();

		if (gameObjectList.find(gameObjectCollideWith) == gameObjectList.end() && (*it).Solve(false))
		{
			gameObjectList.emplace(gameObjectCollideWith);
		}

		it = collisionList.erase(it);
	}
}
