#pragma once
#include "Body2D.h"
#include "Shape.h"
#include "Macros.h"
#include "Collision.h"
#include <deque>
#include <set>
#include <list>

class RigidBody2D;
class GameObj;

class Physic
{
	friend class Game;
public:
	//Return value is alpha used to interpolate render deltatime.
	static bool Update();

	static void SetStep(const float& step);
	[[nodiscard]] static float GetStep();

	static void SetGravity(const float& gravity);
	[[nodiscard]] static const Vector3& GetGravity();

	static void AddRigid2DScript(RigidBody2D* script);
	static void RemoveRigid2DScript(RigidBody2D* script);

	static void AddShape(Shape* shape);
	static void RemoveShape(Shape* _shape);

	static void AddBody(Body2D* body);
	static void RemoveBody(Body2D* body);

	// Use to add a body that have been interacted with another body to the set for calling event;
	static void AddCollidedBody(Body2D* body);
	// Use to remove a body that have been interacted with another body to the set for calling event;
	static void RemoveCollidedBody(Body2D* body);

	static void AddGameObj(GameObj* gameObj);
	static void RemoveGameObj(GameObj* gameObj);
protected:
	Physic() = default;

	static void Reset() noexcept;
	static void Step();
protected:
	static inline float _stepInterpolation = 0.9f;
	static inline float _baseStep = 0.02f;
	static inline float _step = 0.02f;

	static inline float _accumulator = 0;
	static inline Vector3 _gravity = Vector3(0, -10.0f, 0);

	static inline std::deque<GameObj*> _gameObjs;
	static inline std::deque<RigidBody2D*> _rigid2DScripts;
	static inline std::deque<Shape*> _shapes;
	static inline std::deque<Body2D*> _body2D;

	static inline std::list<Collision> _contacts;
	static inline std::set<Body2D*> _collidedBody;
};