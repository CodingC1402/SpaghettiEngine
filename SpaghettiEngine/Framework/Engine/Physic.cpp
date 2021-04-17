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

void Physic::CheckCollision(PBoxCollider object, PBoxCollider other)
{
	float left = other->owner->GetPosition()->x - (object->owner->GetPosition()->x + object->width);
	float top = (other->owner->GetPosition()->y + other->height) - object->owner->GetPosition()->y;
	float right = (other->owner->GetPosition()->x + other->width) - object->owner->GetPosition()->x;
	float bottom = other->owner->GetPosition()->y - (object->owner->GetPosition()->y + object->height);

	if (!(left > 0 || right < 0 || top < 0 || bottom > 0))
	{
		object->owner->Translate(new Vector3(0, 1, 0));
		other->owner->Translate(new Vector3(0, -1, 0));
	}
}
