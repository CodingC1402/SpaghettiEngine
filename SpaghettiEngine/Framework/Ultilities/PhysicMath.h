#pragma once
#include "CornDirectX.h"
#include <algorithm>

class PhysicMath
{
public:
	class Rect
	{
	public:
		float x;
		float y;

		float width;
		float height;

		float vx;
		float vy;

		Rect(float x, float y, float width, float height, float vx, float vy)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->vx = vx;
			this->vy = vy;
		}

		Rect(float x, float y, float width, float height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->vx = 0;
			this->vy = 0;
		}
	};
public:
	static Rect getSweptBroadphaseRect(const Rect& object);
	static float sweptAABB(const Rect& object, const Rect& block, Vector3*& result);
	static bool AABBCheck(Rect object, Rect block);
	static void Response();
};