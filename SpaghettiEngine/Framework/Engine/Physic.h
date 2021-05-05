#pragma once
#include <vector>

typedef class Collider2DScriptBase* PCollider2DScriptBase;
typedef class RigidBody* PRigidBody;
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
	void AddRigidBody(PRigidBody rigidBody);
	void RemoveCollider(PCollider2DScriptBase collider);
	void RemoveRigidBody(PRigidBody rigidbody);
protected:
	void Unload();
	void CheckCollision(PCollider2DScriptBase object, PCollider2DScriptBase block);
protected:
	Physic() = default;
	~Physic();
	static PPhysic __instance;

	std::list<PRigidBody> rigidBodis;
	std::list<PCollider2DScriptBase> colliders;
};