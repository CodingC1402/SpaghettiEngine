#include "Physic.h"

PPhysic Physic::__instance = NULL;

void Physic::Init()
{

}

void Physic::Update()
{
	for (auto object = boxColliders.begin(); object != boxColliders.end(); ++object)
	{
		auto temp = object;
		std::advance(temp, 1);
		for (auto block = temp; block != boxColliders.end(); ++block)
		{
			if (object == block)
				continue;
			CheckCollision(*object, *block);
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
	PhysicMath::Rect obj = PhysicMath::Rect(object->GetPosition().x,
		object->GetPosition().y,
		object->width,
		object->height,
		std::static_pointer_cast<RigidBody>(object->owner->GetScript("RigidBody")).get()->vx,
		std::static_pointer_cast<RigidBody>(object->owner->GetScript("RigidBody")).get()->vy);
	
	PhysicMath::Rect broadphasebox = PhysicMath::getSweptBroadphaseRect(obj);
	
	PhysicMath::Rect blk = PhysicMath::Rect(block->GetPosition().x,
		block->GetPosition().y,
		block->width,
		block->height,
		0,
		0);

	if (PhysicMath::AABBCheck(broadphasebox, blk))
	{
		Vector3* direction;
		float collisiontime = PhysicMath::sweptAABB(obj, blk, direction);
		obj.x += obj.vx * collisiontime;
		obj.y += obj.vy * collisiontime;
	
		float remainingtime = 1.0f - collisiontime;
	
		if (collisiontime < 1.0f)
		{
			obj.vx *= direction->x;
			obj.vy *= direction->y;
			object->owner->Translate(new Vector3(obj.vx, obj.vy * 100, 0));
		}
	}

	return false;
}

