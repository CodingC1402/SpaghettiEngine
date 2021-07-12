#include "QTNode.h"

QTNode::QTNode(float xAxis, float yAxis, float width, float height, NodeType type)
{
	_xAxis = xAxis;
	_yAxis = yAxis;
	_width = width;
	_height = height;
	_type = type;
}

void QTNode::Insert(Shape* shape)
{
	std::bitset<edgeNum> intersect;
	SetIntersectBitset(shape, intersect);

	if (intersect.count() > 2)
		_shapes.push_back(std::pair(shape, intersect));
	else
	{
		unsigned index = 0;
		// Up down left right
		switch (intersect.to_ulong())
		{
		case 0b1001:
			index = static_cast<unsigned>(NodeType::UpRight);
			break;
		case 0b1010:
			index = static_cast<unsigned>(NodeType::UpLeft);
			break;
		case 0b0110:
			index = static_cast<unsigned>(NodeType::DownLeft);
			break;
		case 0b0101:
			index = static_cast<unsigned>(NodeType::DownRight);
			break;
		}

		InsertToSub(shape, index);
	}
	_shapeCounter++;
}

void QTNode::Remove(Shape* shape)
{

}

void QTNode::Clear()
{
	for (auto& node : _subNodes)
	{
		if (node.use_count() > 0)
		{
			node->Clear();
			node.reset();
		}
	}

	_shapes.clear();
}

void QTNode::SetBitSet(const Vector3& point, std::bitset<edgeNum>& intersect)
{
	if (point.x > _yAxis)
		intersect[static_cast<unsigned>(EdgeType::Right)] = true;
	else if (point.x < _yAxis)
		intersect[static_cast<unsigned>(EdgeType::Left)] = true;

	if (point.y > _xAxis)
		intersect[static_cast<unsigned>(EdgeType::Up)] = true;
	else if (point.y < _xAxis)
		intersect[static_cast<unsigned>(EdgeType::Down)] = true;
}

void QTNode::SetIntersectBitset(Shape* shape, std::bitset<edgeNum>& intersect)
{
	SetBitSet(shape->GetBroadPhase().GetWorldTopLeft(), intersect);
	SetBitSet(shape->GetBroadPhase().GetWorldBottomRight(), intersect);
}

void QTNode::InsertToSub(Shape* shape, unsigned index)
{
	if (_subNodes[index].use_count() == 0)
	{
		float xAxis;
		float yAxis;
		float width = _width / 2;
		float height = _height / 2;
		
		NodeType type = static_cast<NodeType>(index);
		switch (type)
		{
		case NodeType::UpRight:
			xAxis = _xAxis + height;
			yAxis = _yAxis + width;
			break;
		case NodeType::UpLeft:
			xAxis = _xAxis + height;
			yAxis = _yAxis - width;
			break;
		case NodeType::DownLeft:
			xAxis = _xAxis - height;
			yAxis = _yAxis - width;
			break;
		case NodeType::DownRight:
			xAxis = _xAxis - height;
			yAxis = _yAxis + width;
			break;
		}

		_subNodes[index] = std::make_shared<QTNode>(xAxis, yAxis, width, height, type);
	}

	_subNodes[index]->Insert(shape);
}
