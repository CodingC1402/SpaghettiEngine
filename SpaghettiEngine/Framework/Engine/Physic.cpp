#include "Physic.h"
#include "Collider2DScriptBase.h"
#include "RigidBody2D.h"

PPhysic Physic::__instance = NULL;

void Physic::Init()
{
	
}

void Physic::Update()
{
	//for (auto object = boxColliders.begin(); object != boxColliders.end(); ++object)
	//{
	//	auto temp = object;
	//	std::advance(temp, 1);
	//	for (auto block = temp; block != boxColliders.end(); ++block)
	//	{
	//		if (object == block)
	//			continue;
	//		CheckCollision(*object, *block);
	//	}
	//}
}

Physic::~Physic()
{
}

Physic* Physic::GetInstance()
{
	if (!__instance)
		__instance = new Physic();
	return __instance;
}

void Physic::AddCollider(PCollider2DScriptBase collider)
{
	colliders.push_back(collider);
}

void Physic::AddRigidBody(PRigidBody rigidbody)
{
	rigidBodis.push_back(rigidbody);
}

void Physic::RemoveCollider(PCollider2DScriptBase collider)
{
	colliders.
}

void Physic::RemoveRigidBody(PRigidBody rigidbody)
{
}

void Physic::Unload()
{
	//rigidBodis.clear();
	//boxColliders.clear();
}

void Physic::CheckCollision(PCollider2DScriptBase object, PCollider2DScriptBase block)
{

}

