#pragma once
#include "Shape.h"

class Collision
{
public:
	void Solve(void);
	void Initialize(void);
	void ApplyImpulse(void);
	void PositionalCorrection(void);
	void InfiniteMassCorrection(void);
protected:
	WShape _shapeA;
	WShape _shapeB;


};