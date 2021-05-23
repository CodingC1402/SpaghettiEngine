#include "Physic.h"
#include "PolygonMath.h"
#include "BoxCollider2D.h"
#include "TriangleCollider2D.h"
#include "GameTimer.h"

PPhysic Physic::__instance = NULL;

ColliderTypes* Physic::map = nullptr;

void Physic::Init()
{	
	if (!map)
		map = new ColliderTypes();
	else
		map->clear();
	
	map->insert(ColliderTypes::value_type(BoxCollider2DID * 100000000 + BoxCollider2DID, &CheckBoxWithBox));
	map->insert(ColliderTypes::value_type(BoxCollider2DID * 100000000 + TriangleCollider2DID, &CheckBoxWithTriangle));
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
		object->_ownerObj->Translate(100, 100, 0);
	}
}

bool IsBoxIntersectTri(BoxCollider2D* object, TriangleCollider2D* block, float& angle)
{
	Vector3 A = block->GetA();
	Vector3 B = block->GetB();
	Vector3 C = block->GetC();

	if (Triangulate::CheckBoxInsideTriangle(A, B, C, object->GetPosition() + object->GetVelocity() * GameTimer::GetDeltaTime(), object->GetSize()))
	{
		Vector3 v = object->GetVelocity();
		Vector3 pos = object->GetPosition() + object->GetVelocity() * GameTimer::GetDeltaTime();
		if (v.y < 0 && object->GetPosition().y > Triangulate::MaxY(A, B, C))
		{
			if (A.y == Triangulate::MaxY(A, B, C))
			{
				if (pos.x < A.x && pos.x + object->GetSize().x > A.x)
				{
					angle = 0;
				}
				else if (pos.x < A.x && pos.x + object->GetSize().x < A.x)
				{
					if (B.x < C.x)
						angle = std::atanf((B.y - A.y) / (B.x - A.x));
					else if (B.x > C.x)
						angle = std::atanf((C.y - A.y) / (C.x - A.x));
					else if (B.y > C.y)
						angle = std::atanf((B.y - A.y) / (B.x - A.x));
					else
						angle = std::atanf((C.y - A.y) / (C.x - A.x));
				}
			}
			else if (B.y == Triangulate::MaxY(A, B, C))
			{

			}
			else
			{

			}
		}
		else if (v.y > 0 && object->GetPosition().y < Triangulate::MinY(A, B, C))
		{

		}

		return true;
	}

	return false;
}

void Physic::CheckBoxWithTriangle(PCollider2DScriptBase alpha, PCollider2DScriptBase beta)
{
	auto object = dynamic_cast<BoxCollider2D*>(alpha);
	auto block = dynamic_cast<TriangleCollider2D*>(beta);

	float angle = -1;

	if (IsBoxIntersectTri(object, block, angle))
	{
		object->_ownerObj->Translate(0, 10, 0);
		object->SetVelocity(Vector3(0, 0, 0));
	}
}



void Physic::Unload()
{
	rigidBodis.clear();
	colliders.clear();
}
