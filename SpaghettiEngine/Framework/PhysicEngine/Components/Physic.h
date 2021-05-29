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
	static void Update(const float& time);

	static void SetStep(const float& step);
	[[nodiscard]] static float GetStep();

	static void SetGravity(const float& gravity);
	[[nodiscard]] static const Vector3& GetGravity();

	static void AddShape(Shape* shape);
	static void RemoveShape(Shape* _shape);

	static void AddBody(Body2D* body);
	static void RemoveBody(Body2D* body);
protected:
	static void Step();
protected:
	static inline float _step = 0.02f;
	static inline float _accumulator = 0;
	static inline Vector3 _gravity = Vector3(0, -10.0f, 0);

	static inline std::deque<Shape*> _shapes;
	static inline std::deque<Body2D*> _body2D;
	static inline std::list<Collision> _contacts;
};