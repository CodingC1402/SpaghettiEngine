#include "Physic.h"

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

void Physic::AddBoxCollider(PBoxCollider newCollider)
{
	//boxColliders.push_back(newCollider);
}

void Physic::AddRigidBody(PRigidBody newRigidBody)
{
	//rigidBodis.push_back(newRigidBody);
}

void Physic::Unload()
{
	//rigidBodis.clear();
	//boxColliders.clear();
}

void Physic::CheckCollision(PBoxCollider object, PBoxCollider block)
{

}

