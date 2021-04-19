#include "Physic.h"

PPhysic Physic::__instance = NULL;

void Physic::Init()
{

}

void Physic::Update()
{
	for (const auto& rigidbody : rigidBodis)
	{
		rigidbody->Update();
	}

	for (const auto& object : boxColliders)
	{
		for (const auto& block : boxColliders)
		{
			if (object == block)
				continue;
			CheckCollision(object, block);
		}
	}
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
	boxColliders.push_back(newCollider);
}

void Physic::AddRigidBody(PRigidBody newRigidBody)
{
	rigidBodis.push_back(newRigidBody);
}

void Physic::Unload()
{
	rigidBodis.clear();
	boxColliders.clear();
}

bool Physic::CheckCollision(PBoxCollider object, PBoxCollider block)
{
	Box broadphasebox = GetSweptBroadphaseBox(box);

	if (AABBCheck(broadphasebox, block))
	{
		float normalx, normaly;
		float collisiontime = SweptAABB(box, block, out normalx, out normaly);
		box.x += box.vx * collisiontime;
		box.y += box.vy * collisiontime;
		if (collisiontime < 1.0f)
		{
			// perform response here 
		}
	}
}

