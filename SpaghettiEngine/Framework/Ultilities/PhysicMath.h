#pragma once
#include <vector>
#include "Collider2DScriptBase.h"
#include "Plane2D.h"

class Quadtree
{
private:
	int MAX_OBJECTS = 10;
	int MAX_LEVELS = 5;

	int level;
	std::list<PCollider2DScriptBase> objects;

	Plane2D::Rectangle* bounds;
	std::vector<Quadtree*> nodes;
private:
	void Split();
	int GetIndex(Plane2D::Rectangle* pRect);
public:
	Quadtree(int pLevel, Plane2D::Rectangle* pBounds);
	void Clear();
	void Insert(Plane2D::Rectangle* pRect);
	std::list<PCollider2DScriptBase> GetList(std::list<PCollider2DScriptBase> returnObj, Plane2D::Rectangle* pRect);
};