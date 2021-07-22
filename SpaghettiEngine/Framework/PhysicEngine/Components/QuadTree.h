#pragma once
#include "QTNode.h"
#include  <deque>

class QuadTree
{
public:
	void Clear();
	void Insert(std::deque<Shape*>& shapes);
	void CreateCollisionList(std::list<Collision>& collisionList);
	void CreateCollisionListWithShape(std::list<Collision>& collisionList, Shape* shape);

	void Draw();
private:
	SQTNode _root;
};