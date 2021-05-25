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
		if (object->GetID() == BoxCollider2DID)
		{
			CheckBoxWithMap(object, tilemap);
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

void Physic::CheckBoxWithTriangle(PCollider2DScriptBase alpha, PCollider2DScriptBase beta)
{
	auto object = dynamic_cast<BoxCollider2D*>(alpha);
	auto block = dynamic_cast<TriangleCollider2D*>(beta);

	float angle = -1;

	if (Triangulate::CheckBoxInsideTriangle(block->GetA(), block->GetB(), block->GetC(),
		object->GetPosition() + object->GetVelocity() * GameTimer::GetDeltaTime(), object->GetSize())) 
	{
		object->_ownerObj->Translate(0, 10, 0);
		object->SetVelocity(Vector3(0, 0, 0));
	}
}

void Physic::CheckBoxWithMap(PCollider2DScriptBase object, std::list<PCollider2DScriptBase> map)
{
	auto obj = dynamic_cast<BoxCollider2D*>(object);
	std::vector<TriangleCollider2D*> list;

	for (auto col : map)
	{
		auto temp = dynamic_cast<TriangleCollider2D*>(col);

		if (Triangulate::CheckBoxInsideTriangle(temp->GetA(), temp->GetB(), temp->GetC(),
			obj->GetPosition() + obj->GetVelocity() * GameTimer::GetDeltaTime(), obj->GetSize()))
		{
			list.push_back(temp);
		}
	}

	if (list.size() > 0)
	{
		Vector3 alpha = list[0]->GetA();
		Vector3 beta;
		Vector3 gamma;

		std::vector<Vector3> pts;
		std::vector<Vector3>::iterator it;
		pts.push_back(alpha);

		for (TriangleCollider2D* temp : list)
		{
			it = std::find(pts.begin(), pts.end(), temp->GetA());
			if (it == pts.end())
				pts.push_back(temp->GetA());

			it = std::find(pts.begin(), pts.end(), temp->GetB());
			if (it == pts.end())
				pts.push_back(temp->GetB());

			it = std::find(pts.begin(), pts.end(), temp->GetC());
			if (it == pts.end())
				pts.push_back(temp->GetC());
		}

		Vector3 fpos = obj->GetPosition() + obj->GetVelocity() * GameTimer::GetDeltaTime();
		if (obj->GetVelocity().y <= 0)
		{
			for (Vector3 temp : pts)
			{
				if (temp.x >= obj->GetPosition().x && temp.x <= fpos.x + obj->GetSize().x && temp.y >= alpha.y)
					alpha = temp;
			}

			beta = Vector3(0, INT_MIN, 0);
			gamma = Vector3(0, INT_MIN, 0);

			for (Vector3 temp : pts)
			{
				if (temp != alpha && temp.x < alpha.x && temp.y > beta.y)
					beta = temp;

				if (temp != alpha && temp.x > alpha.x && temp.y > gamma.y)
					gamma = temp;
			}

			if (beta.y == INT_MIN)
				beta = gamma;
			if (beta.y != INT_MIN && gamma.y != INT_MIN)
			{
				if (Triangulate::CountLineIntersect(alpha, gamma, obj->GetPosition(), obj->GetSize(), obj->GetVelocity()) >
					Triangulate::CountLineIntersect(alpha, beta, obj->GetPosition(), obj->GetSize(), obj->GetVelocity()))
				{
					beta = gamma;
				}
			}

			float angle;
			if (alpha.x == beta.x)
				angle = 90;

			angle = std::atanf(abs((alpha.y - beta.y) / (alpha.x - beta.x)));

			obj->_ownerObj->Translate(-obj->GetVelocity() * GameTimer::GetDeltaTime());
			obj->_ownerObj->Translate(obj->GetVelocity().x * std::sin(angle) * GameTimer::GetDeltaTime(), 
				obj->GetVelocity().y * std::cos(angle) * GameTimer::GetDeltaTime(), 0);
			
			obj->AddForce(Vector3(0, -obj->GetVelocity().y * std::cos(angle), 0));
		}
	}
}

void Physic::Unload()
{
	rigidBodis.clear();
	colliders.clear();
}
