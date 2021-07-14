#include "QuadTree.h"

void QuadTree::Clear()
{
	if (_root.use_count() > 0)
		_root->Clear();
}

void QuadTree::Insert(std::deque<Shape*>& shapes)
{
	Clear();
	if (shapes.empty())
		return;

	Vector3 topLeft = shapes[0]->GetBroadPhase().GetWorldTopLeft();
	Vector3 bottomRight = shapes[0]->GetBroadPhase().GetWorldBottomRight();

	float minX = topLeft.x;
	float maxX = bottomRight.x;
	float minY = bottomRight.y;
	float maxY = topLeft.y;
	
	for (auto it = ++shapes.begin(); it != shapes.end(); ++it)
	{
		topLeft = (*it)->GetBroadPhase().GetWorldTopLeft();
		bottomRight = (*it)->GetBroadPhase().GetWorldBottomRight();

		minX = minX > topLeft.x		? topLeft.x		: minX;
		maxX = maxX < bottomRight.x ? bottomRight.x : maxX;
		minY = minY > bottomRight.y ? bottomRight.y : minY;
		maxY = maxY < topLeft.y		? topLeft.y		: maxY;
	}

	_root = std::make_shared<QTNode>((minY + maxY) / 2.0f, (minX + maxX) / 2.0f, maxX - minX, maxY - minY, QTNode::NodeType::Root);
	for (auto& shape : shapes)
	{
		_root->Insert(shape);
	}
}

void QuadTree::CreateCollisionList(std::list<Collision>& collisionList)
{
	_root->CreateCollisionList(collisionList);
}

void QuadTree::CreateCollisionListWithShape(std::list<Collision>& collisionList, Shape* shape)
{
	if (_root.use_count() > 0)
		_root->CreateCollisionListWithShape(shape, collisionList);
}

void QuadTree::Draw()
{
	if (_root.use_count() > 0)
		_root->Draw();
}
