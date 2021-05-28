#include "Collision.h"

Collision::Collision(WShape A, WShape B)
{
	_shapeA = A;
	_shapeB = B;
}

bool Collision::Solve()
{
	return _dispatcher(_shapeA, _shapeB);
}
