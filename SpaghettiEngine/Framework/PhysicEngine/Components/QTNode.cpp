#include "QTNode.h"

// Right, Left, Down Up
QTNode::QTNode(float xAxis, float yAxis, float width, float height, NodeType type)
{
	_xAxis = xAxis;
	_yAxis = yAxis;
	_width = width;
	_height = height;
	_type = type;
}

void QTNode::CreateCollisionList(std::list<Collision>& collisionList)
{
	for (auto& shape : _shapes)
		CreateCollisionListWithShape(shape.first, shape.second, collisionList);

	for (auto& node : _subNodes)
		if (node.use_count() > 0)
			node->CreateCollisionList(collisionList);
}

void QTNode::CheckSubNodeAndCallCreateCollision(Shape* shape, std::list<Collision>& collisionList, QTNode::NodeType type)
{
	unsigned index = static_cast<unsigned>(type);
	if (_subNodes[index].use_count() > 0)
		_subNodes[index]->CreateCollisionListWithShape(shape, collisionList);
}
void QTNode::CreateCollisionListWithEdgeIndex(unsigned index, Shape* shape, std::list<Collision>& collisionList)
{
	for (auto& collideShape : _shapes)
	{
		if (collideShape.second[index])
			collisionList.push_back(Collision(shape, collideShape.first));
	}
}
void QTNode::CreateCollisionListWithEdgeIndex(const std::vector<unsigned>& indexes, Shape* shape, std::list<Collision>& collisionList)
{
	bool isCollide = true;
	for (auto& collideShape : _shapes)
	{
		isCollide = true;
		for (const auto& index : indexes)
			isCollide &= collideShape.second[index];
		if (isCollide)
			collisionList.push_back(Collision(shape, collideShape.first));
	}
}
void QTNode::CreateCollisionListWithShape(Shape* shape, std::list<Collision>& collisionList)
{
	std::bitset<edgeNum> intersect;
	SetIntersectBitset(shape, intersect);

	CreateCollisionListWithShape(shape, intersect, collisionList);
}

void QTNode::CreateCollisionListWithShape(Shape* shape, const std::bitset<edgeNum>& intersect, std::list<Collision>& collisionList)
{
	if (intersect.count() > 2)
	{
		// Right, Left, Down Up
		switch (intersect.to_ulong())
		{
		case 0b1111:
			for (int i = 0; i < edgeNum; i++)
			{
				if (_subNodes[i].use_count() > 0)
					_subNodes[i]->CreateCollisionListWithShape(shape, collisionList);
			}
			break;
		case 0b0111:
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::UpLeft);
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::DownLeft);
			CreateCollisionListWithEdgeIndex(3u, shape, collisionList);
			break;
		case 0b1011:
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::UpRight);
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::DownRight);
			CreateCollisionListWithEdgeIndex(2u, shape, collisionList);
			break;
		case 0b1101:
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::UpLeft);
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::UpRight);
			CreateCollisionListWithEdgeIndex(1u, shape, collisionList);
			break;
		case 0b1110:
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::DownLeft);
			CheckSubNodeAndCallCreateCollision(shape, collisionList, NodeType::DownRight);
			CreateCollisionListWithEdgeIndex(0u, shape, collisionList);
			break;
		}
	}
	else
	{
		// Right, Left, Down Up
		switch (_type)
		{
		case NodeType::DownLeft:
			CreateCollisionListWithEdgeIndex({ 1, 2 }, shape, collisionList);
			break;
		case NodeType::DownRight:
			CreateCollisionListWithEdgeIndex({ 0, 2 }, shape, collisionList);
			break;
		case NodeType::UpLeft:
			CreateCollisionListWithEdgeIndex({ 1, 3 }, shape, collisionList);
			break;
		case NodeType::UpRight:
			CreateCollisionListWithEdgeIndex({ 0, 3 }, shape, collisionList);
			break;
		}

		unsigned index = GetIndexFromBitSet(intersect);
		if (_subNodes[index].use_count() > 0)
			_subNodes[index]->CreateCollisionListWithShape(shape, collisionList);
	}
}

void QTNode::Draw()
{
	DebugRenderer::DrawRectangle(Vector3(_yAxis - _width / 2.0f, _xAxis + _height / 2.0f, 0), _width, _height, Matrix4::GetDiagonalMatrix());
	for (auto& node : _subNodes)
	{
		if (node.use_count() > 0)
			node->Draw();
	}
}

void QTNode::Insert(Shape* shape)
{
	std::bitset<edgeNum> intersect;
	SetIntersectBitset(shape, intersect);

	if (intersect.count() > 2)
		_shapes.insert(std::pair(shape, intersect));
	else
	{
		unsigned index = GetIndexFromBitSet(intersect);
		InsertToSub(shape, index);
	}
	_shapeCounter++;
}

void QTNode::Remove(Shape* shape)
{
	std::bitset<edgeNum> intersect;
	SetIntersectBitset(shape, intersect);

	if (intersect.count() > 2)
		_shapes.erase(shape);
	else
	{
		unsigned index = GetIndexFromBitSet(intersect);
		if (_subNodes[index].use_count() > 0)
		{
			_subNodes[index]->Remove(shape);
			if (_subNodes[index]->_shapeCounter == 0)
				_subNodes[index].reset();
		}
	}

	_shapeCounter--;
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
		float width = _width / 4.0f;
		float height = _height / 4.0f;
		
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

		_subNodes[index] = std::make_shared<QTNode>(xAxis, yAxis, width * 2.0f, height * 2.0f, type);
	}

	_subNodes[index]->Insert(shape);
}

unsigned QTNode::GetIndexFromBitSet(const std::bitset<edgeNum>& intersect) const noexcept
{
	// Right, Left, Down Up
	switch (intersect.to_ulong())
	{
	case 0b1001:
		return static_cast<unsigned>(NodeType::UpRight);
	case 0b0101:
		return static_cast<unsigned>(NodeType::UpLeft);
	case 0b0110:
		return static_cast<unsigned>(NodeType::DownLeft);
	case 0b1010:
		return static_cast<unsigned>(NodeType::DownRight);
	default:
		return static_cast<unsigned>(NodeType::Invalid);
	}
}
