#pragma once
#include <vector>
#include "Collider2DScriptBase.h"
#include "RigidBody2D.h"

typedef class Collider2DScriptBase* PCollider2DScriptBase;
typedef class RigidBody2D* PRigidBody2D;
typedef class Physic* PPhysic;

class Physic
{
	friend class Game;
	friend class SceneManager;
public:
	void Init();
	void Update();
	static Physic* GetInstance();

	void AddCollider(PCollider2DScriptBase collider);
	void AddRigidBody(PRigidBody2D rigidBody);

	void RemoveCollider(PCollider2DScriptBase collider);
	void RemoveRigidBody(PRigidBody2D rigidbody);
protected:
	void Unload();
	void CheckCollision(PCollider2DScriptBase object, PCollider2DScriptBase block);
protected:
	Physic() = default;
	~Physic();
	static PPhysic __instance;

	std::list<PRigidBody2D> rigidBodis;
	std::list<PCollider2DScriptBase> colliders;
};