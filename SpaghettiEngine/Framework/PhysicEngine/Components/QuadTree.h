#pragma once
#include "QTNode.h"
#include  <deque>

class QuadTree
{
public:
	void Clear();
	void Insert(std::deque<Shape*>& shapes);
	void CreateCollisionList(std::list<Collision>& collisionList);
private:
	SQTNode _root;
};