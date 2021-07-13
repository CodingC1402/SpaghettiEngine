#pragma once
#include "Macros.h"
#include "Shape.h"
#include "Collision.h"
#include "DebugRenderer.h"
#include <bitset>
#include <map>

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
		All
	};
	enum class NodeType
	{
		UpLeft,
		UpRight,
		DownLeft,
		DownRight,
		Root,
		Invalid
	};
public:
	QTNode(float xAxis, float yAxis, float width, float height, NodeType type);

	// Use this to create all the possible collision.
	void CreateCollisionList(std::list<Collision>& collisionList);

	// Use this if you want to create a collision list from a shape that doesn't owned by the node.
	void CreateCollisionListWithShape(Shape* shape, std::list<Collision>& collisionList);

	void Draw();
	void Insert(Shape* shape);
	void Remove(Shape* shape);
	void Clear();
private:
	// return whether it should be in sub or not
	void SetBitSet(const Vector3& point, std::bitset<edgeNum>& intersect);
	void SetIntersectBitset(Shape* shape, std::bitset<edgeNum>& intersect);
	void InsertToSub(Shape* shape, unsigned index);

	void CreateCollisionListWithShape(Shape* shape, const std::bitset<edgeNum>& intersect, std::list<Collision>& collisionList, unsigned startIndex = 0);
	void CheckSubNodeAndCallCreateCollision(Shape* shape, std::list<Collision>& collisionList, QTNode::NodeType type);

	/// <summary>
	/// Use this to create collision from a pair of shape that is on the edge
	/// </summary>
	/// <param name="index">The edge index in bitset that the function have to check</param>
	/// <param name="shape"></param>
	/// <param name="collisionList"></param>
	/// <param name="shapeIndex">Tell the function where to start counting from</param>
	void CreateCollisionListWithEdgeIndex(unsigned index, Shape* shape, std::list<Collision>& collisionList, unsigned shapeIndex = 0);
	void CreateCollisionListWithEdgeIndex(const std::vector<unsigned>& indexes, Shape* shape, std::list<Collision>& collisionList);

	[[nodiscard]] unsigned GetIndexFromBitSet(const std::bitset<edgeNum>& intersect) const noexcept;
private:
	float _xAxis;
	float _yAxis;
	float _width;
	float _height;
	NodeType _type;

	unsigned _shapeCounter = 0;
	SQTNode _subNodes[4];

	// Bit set is the edge that the BroadPhase rect intersect
	std::map<Shape*, std::bitset<edgeNum>> _shapes;
};