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

	Vector3 topLeft = shapes[0]->GetBroadPhase().GetTopLeft();
	Vector3 bottomRight = shapes[0]->GetBroadPhase().GetBottomRight();

	float minX = topLeft.x;
	float maxX = bottomRight.x;
	float minY = bottomRight.y;
	float maxY = topLeft.y;
	
	for (auto it = ++shapes.begin(); it != shapes.end(); ++it)
	{
		topLeft = (*it)->GetBroadPhase().GetTopLeft();
		bottomRight = (*it)->GetBroadPhase().GetBottomRight();

		minX = minX > topLeft.x		? topLeft.x		: minX;
		maxX = maxX < bottomRight.x ? bottomRight.x : maxX;
		minY = minY > bottomRight.y ? bottomRight.y : minY;
		maxY = maxY < topLeft.y		? topLeft.y		: maxY;
	}

	_root = std::make_shared<QTNode>((minY + maxY) / 2.0f, (minX + maxX) / 2.0f, maxX - minX, maxY - minY, QTNode::NodeType::Root);
}
