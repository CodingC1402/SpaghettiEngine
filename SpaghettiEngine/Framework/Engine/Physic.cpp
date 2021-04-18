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

	for (const auto& boxCollider : boxColliders)
	{
		boxCollider->Update();
	}

	for (const auto& object : boxColliders)
	{
		for (const auto& other : boxColliders)
		{
			if (object == other)
				continue;
			CheckCollision(object, other);
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

void Physic::CheckCollision(PBoxCollider object, PBoxCollider other)
{
	float left = other->x - (object->x + object->width - object->width / 2);
	float top = (other->y + other->height) - (object->y - object->height / 2);
	float right = (other->x + other->width) - (object->x - object->width / 2);
	float bottom = other->y - (object->y + object->height - object->height / 2);

	if (!(left > 0 || right < 0 || top < 0 || bottom > 0))
	{
		object->owner->Translate(new Vector3(0, 1, 0));
		other->owner->Translate(new Vector3(0, -1, 0));
	}
}
