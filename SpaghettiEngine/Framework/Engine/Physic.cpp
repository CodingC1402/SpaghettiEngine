#include "Physic.h"
#include "BoxCollider2D.h"

PPhysic Physic::__instance = NULL;

ColliderTypes* Physic::map = nullptr;

void Physic::Init()
{	
	if (!map)
		map = new ColliderTypes();
	else
		map->clear();
	
	map->insert(ColliderTypes::value_type(BoxCollider2DID * 100000000 + BoxCollider2DID, &CheckBoxWithBox));
}

void Physic::Update()
{
	long lID;

	for (auto object : colliders)
	{
		for (auto block : tilemap)
		{
			if (object->GetID() > block->GetID())
			{
				lID = block->GetID() * 100000000 + object->GetID();
				auto a = map->find(lID);
				a->second(block, object);
			}
			else
			{
				lID = object->GetID() * 100000000 + block->GetID();
				auto a = map->find(lID);
				a->second(object, block);
			}
		}
	}

	for (auto object = colliders.begin(); object != colliders.end(); ++object)
	{
		auto temp = object;
		std::advance(temp, 1);
		for (auto block = temp; block != colliders.end(); ++block)
		{
			if (object == block)
				continue;
			
			if ((*object)->GetID() > (*block)->GetID())
			{
				lID = (*block)->GetID() * 100000000 + (*object)->GetID();
				auto a = map->find(lID);
				a->second(*block, *object);
			}
			else
			{
				lID = (*object)->GetID() * 100000000 + (*block)->GetID();
				auto a = map->find(lID);
				a->second(*object, *block);
			}
		}
	}
}

Physic::~Physic()
{
	Unload();
}

Physic* Physic::GetInstance()
{
	if (!__instance)
		__instance = new Physic();
	return __instance;
}

void Physic::AddMapCollider(PCollider2DScriptBase collider)
{
	tilemap.emplace_back(collider);
}

void Physic::AddCollider(PCollider2DScriptBase collider)
{
	colliders.emplace_back(collider);
}

void Physic::AddRigidBody(PRigidBody2D rigidbody)
{
	rigidBodis.emplace_back(rigidbody);
}

void Physic::RemoveCollider(PCollider2DScriptBase collider)
{
	colliders.remove(collider);
}

void Physic::RemoveRigidBody(PRigidBody2D rigidbody)
{
	rigidBodis.remove(rigidbody);
}

ColliderTypes* Physic::GetMap()
{
	return map;
}

void Physic::CheckBoxWithBox(PCollider2DScriptBase alpha, PCollider2DScriptBase beta)
{
	auto object = dynamic_cast<BoxCollider2D*>(alpha);
	auto block = dynamic_cast<BoxCollider2D*>(beta);
	
	float topEdge1 = object->GetPosition().y + object->GetSize().y;
	float rightEdge1 = object->GetPosition().x + object->GetSize().x;
	float leftEdge1 = object->GetPosition().x;
	float bottomEdge1 = object->GetPosition().y;
	float topEdge2 = block->GetPosition().y + block->GetSize().y;
	float rightEdge2 = block->GetPosition().x + block->GetSize().x;
	float leftEdge2 = block->GetPosition().x;
	float bottomEdge2 = block->GetPosition().y;

	if (leftEdge1 < rightEdge2 && rightEdge1 > leftEdge2 && bottomEdge1 < topEdge2 && topEdge1 > bottomEdge2) {
		object->_ownerObj->Translate(100, 100, 100);
	}
}

void Physic::Unload()
{
	rigidBodis.clear();
	colliders.clear();
}

