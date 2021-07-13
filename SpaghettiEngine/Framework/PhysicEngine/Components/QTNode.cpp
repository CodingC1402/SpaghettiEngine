#include "QTNode.h"
#include "Setting.h"

constexpr int DrawColor = 0xFFFF00FF;
constexpr int AxisesColor = 0xFFFFFF00;

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
	int offSetIndex = 0;
	auto it = _shapes.begin();
	while (offSetIndex < static_cast<int>(_shapes.size()))
	{
		CreateCollisionListWithShape((*it).first, (*it).second, collisionList, ++offSetIndex);
		++it;
	}

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

void QTNode::CreateCollisionListWithEdgeIndex(unsigned index, Shape* shape, std::list<Collision>& collisionList, unsigned shapeIndex)
{
	auto it = _shapes.begin();
	std::advance(it, shapeIndex);
	for (; it != _shapes.end(); ++it)
	{
		// If all then it could collide with all shape that is intersect with axises 
		if (index == static_cast<unsigned>(EdgeType::All) || (*it).second[index])
			collisionList.push_back(Collision(shape, (*it).first));
	}
}

void QTNode::CreateCollisionListWithEdgeIndex(const std::vector<unsigned>& indexes, Shape* shape, std::list<Collision>& collisionList)
{
	bool isCollide = true;
	for (auto& collideShape : _shapes)
	{
		isCollide = true;
		for (const auto& index : indexes)
			isCollide = isCollide && collideShape.second[index];
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

void QTNode::CreateCollisionListWithShape(Shape* shape, const std::bitset<edgeNum>& intersect, std::list<Collision>& collisionList, unsigned startIndex)
{
	if (intersect.count() > 2)
	{
		NodeType node1 = NodeType::Invalid;
		NodeType node2 = NodeType::Invalid;
		unsigned bitIndex = 0;

		// Right, Left, Down, Up
		switch (intersect.to_ulong())
		{
		case 0b1111:
			for (int i = 0; i < edgeNum; i++)
			{
				if (_subNodes[i].use_count() > 0)
					_subNodes[i]->CreateCollisionListWithShape(shape, collisionList);
			}
			CreateCollisionListWithEdgeIndex(static_cast<unsigned>(EdgeType::All), shape, collisionList, startIndex);
			return;
		case 0b1011:
			node1 = NodeType::UpRight;
			node2 = NodeType::DownRight;
			bitIndex = static_cast<unsigned>(EdgeType::Right);
			break;
		case 0b0111:
			node1 = NodeType::UpLeft;
			node2 = NodeType::DownLeft;
			bitIndex = static_cast<unsigned>(EdgeType::Left);
			break;
		case 0b1110:
			node1 = NodeType::DownLeft;
			node2 = NodeType::DownRight;
			bitIndex = static_cast<unsigned>(EdgeType::Down);
			break;
		case 0b1101:
			node1 = NodeType::UpLeft;
			node2 = NodeType::UpRight;
			bitIndex = static_cast<unsigned>(EdgeType::Up);
			break;
		}

		CheckSubNodeAndCallCreateCollision(shape, collisionList, node1);
		CheckSubNodeAndCallCreateCollision(shape, collisionList, node2);
		CreateCollisionListWithEdgeIndex(bitIndex, shape, collisionList, startIndex);
	}
	else
	{
		// Right, Left, Down Up
		std::vector<unsigned> bitIndexes;
		for (int i = 0; i < intersect.size(); i++)
			if (intersect[i])
				bitIndexes.push_back(i);
		CreateCollisionListWithEdgeIndex(bitIndexes, shape, collisionList);

		unsigned index = GetIndexFromBitSet(intersect);
		if (_subNodes[index].use_count() > 0)
			_subNodes[index]->CreateCollisionListWithShape(shape, collisionList);
	}
}

void QTNode::Draw()
{
	if constexpr (!Setting::IsDebugMode())
		return;

	if constexpr (Setting::IsDrawQuadTree())
	{
		DebugRenderer::DrawRectangle(
			Vector3(_yAxis - _width / 2.0f, _xAxis + _height / 2.0f, 0),
			_width,
			_height,
			Matrix4::GetDiagonalMatrix(),
			DrawColor);
	}

	if constexpr (Setting::IsDrawQuadTreeGrid())
	{
		DebugRenderer::DrawLine(
			Vector3(_yAxis, _xAxis + _height / 2.0f, 0), 
			Vector3(_yAxis, _xAxis - _height / 2.0f, 0), 
			Matrix4::GetDiagonalMatrix(), 
			AxisesColor);

		DebugRenderer::DrawLine(
			Vector3(_yAxis + _width / 2.0f, _xAxis, 0), 
			Vector3(_yAxis - _width / 2.0f, _xAxis, 0), 
			Matrix4::GetDiagonalMatrix(), 
			AxisesColor);
	}

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
