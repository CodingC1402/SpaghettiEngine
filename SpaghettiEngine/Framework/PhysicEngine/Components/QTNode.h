#pragma once
#include "Macros.h"
#include "Shape.h"
#include <bitset>
#include <list>

CLASS_FORWARD_DECLARATION(QTNode);
constexpr auto edgeNum = 4;

class QTNode
{
public:
	enum class EdgeType
	{
		Up,
		Down,
		Left,
		Right,
		Sub
	};
	enum class NodeType
	{
		UpLeft,
		UpRight,
		DownLeft,
		DownRight,
		Root
	};
public:
	QTNode(float xAxis, float yAxis, float width, float height, NodeType type);

	void Insert(Shape* shape);
	void Remove(Shape* shape);
	void Clear();
private:
	// return whether it should be in sub or not
	void SetBitSet(const Vector3& point, std::bitset<edgeNum>& intersect);
	void SetIntersectBitset(Shape* shape, std::bitset<edgeNum>& intersect);
	void InsertToSub(Shape* shape, unsigned index);
private:
	float _xAxis;
	float _yAxis;
	float _width;
	float _height;
	NodeType _type;

	unsigned _shapeCounter = 0;
	SQTNode _subNodes[4];

	// Parent node
	WQTNode _parent;
	// Index in parent container
	unsigned _index;

	// Bit set is the edge that the BroadPhase rect intersect
	std::list<std::pair<Shape*, std::bitset<edgeNum>>> _shapes;
};