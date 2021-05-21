#include "PhysicMath.h"

void Quadtree::Split()
{
	int subWidth = bounds->w / 2;
	int subHeight = bounds->h / 2;
	int x = bounds->x;
	int y = bounds->y;

	nodes[0] = new Quadtree(level + 1, new Plane2D::Rectangle(x + subWidth, y, subWidth, subHeight));
	nodes[1] = new Quadtree(level + 1, new Plane2D::Rectangle(x, y, subWidth, subHeight));
	nodes[2] = new Quadtree(level + 1, new Plane2D::Rectangle(x, y + subHeight, subWidth, subHeight));
	nodes[3] = new Quadtree(level + 1, new Plane2D::Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));
}

int Quadtree::GetIndex(Plane2D::Rectangle* pRect)
{
	int index = -1;
	double verticalMidpoint = bounds->x + (bounds->w / 2);
	double horizontalMidpoint = bounds->y + (bounds->h / 2);

	// Object can completely fit within the top quadrants
	bool topQuadrant = (pRect->y < horizontalMidpoint && pRect->y + pRect->h < horizontalMidpoint);
	// Object can completely fit within the bottom quadrants
	bool bottomQuadrant = (pRect->y > horizontalMidpoint);

	// Object can completely fit within the left quadrants
	if (pRect->x < verticalMidpoint && pRect->x + pRect->w < verticalMidpoint) {
		if (topQuadrant) {
			index = 1;
		}
		else if (bottomQuadrant) {
			index = 2;
		}
	}
	// Object can completely fit within the right quadrants
	else if (pRect->x > verticalMidpoint) {
		if (topQuadrant) {
			index = 0;
		}
		else if (bottomQuadrant) {
			index = 3;
		}
	}

	return index;
}

Quadtree::Quadtree(int pLevel, Plane2D::Rectangle* pBounds)
{
	level = pLevel;
	objects = std::list<PCollider2DScriptBase>();
	bounds = pBounds;
	nodes = std::vector<Quadtree*>(4);
}

void Quadtree::Clear()
{
	objects.clear();

	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i] != nullptr) {
			nodes[i]->Clear();
			nodes[i] = nullptr;
		}
	}
}

void Quadtree::Insert(Plane2D::Rectangle* pRect)
{
	//if (nodes[0] != null) {
	//	int index = getIndex(pRect);
	//
	//	if (index != -1) {
	//		nodes[index].insert(pRect);
	//
	//		return;
	//	}
	//}
	//
	//objects.add(pRect);
	//
	//if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
	//	if (nodes[0] == null) {
	//		split();
	//	}
	//
	//	int i = 0;
	//	while (i < objects.size()) {
	//		int index = getIndex(objects.get(i));
	//		if (index != -1) {
	//			nodes[index].insert(objects.remove(i));
	//		}
	//		else {
	//			i++;
	//		}
	//	}
	//}
}

std::list<PCollider2DScriptBase> Quadtree::GetList(std::list<PCollider2DScriptBase> returnObj, Plane2D::Rectangle* pRect)
{
	int index = GetIndex(pRect);
	if (index != -1 && nodes[0] != nullptr) {
		nodes[index]->GetList(returnObj, pRect);
	}

	//returnObj.addAll(objects);

	return returnObj;
}
