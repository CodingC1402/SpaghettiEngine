#pragma once
#include "Body2D.h"
#include "Shape.h"
#include "Macros.h"
#include "Collision.h"
#include <deque>

class Physic
{
public:
	//Return value is alpha used to interpolate render deltatime.
	float Update();
	void Step();
	static void SetStep(const float step);
protected:
	static inline float _step = 0.02f;
	static inline float _accumulator = 0;

	static inline std::deque<WShape> _shapes;
	static inline std::list<Collision> _contacts;
};