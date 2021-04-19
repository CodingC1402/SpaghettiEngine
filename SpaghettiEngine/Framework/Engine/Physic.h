#pragma once
#include "PhysicMath.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include <memory.h>
#include <vector>

typedef class BoxCollider* PBoxCollider;
typedef class RigidBody* PRigidBody;
typedef class Physic* PPhysic;

class Physic
{
	friend class Game;
public:
	void Init();
	void Update();
	static Physic* GetInstance();
	void AddBoxCollider(PBoxCollider newCollider);
	void AddRigidBody(PRigidBody newRigidBody);
protected:
	void Unload();
	bool CheckCollision(PBoxCollider object, PBoxCollider block);
protected:
	Physic() = default;
	~Physic();
	static PPhysic __instance;
private:
	std::list<PRigidBody> rigidBodis;
	std::list<PBoxCollider> boxColliders;
};