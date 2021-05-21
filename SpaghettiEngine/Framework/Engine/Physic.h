#pragma once
#include <vector>
#include "Collider2DScriptBase.h"
#include "RigidBody2D.h"

constexpr long BoxCollider2DID = 00000000;
constexpr long EdgeCollider2DID = 00000001;
constexpr long CircleCollider2DID = 00000002;


typedef class Collider2DScriptBase* PCollider2DScriptBase;
typedef class RigidBody2D* PRigidBody2D;
typedef class Physic* PPhysic;

typedef std::map<long, void (*)(PCollider2DScriptBase, PCollider2DScriptBase)> ColliderTypes;

class Physic
{
	friend class Game;
	friend class SceneManager;
public:
	void Init();
	void Update();
	static Physic* GetInstance();

	void AddMapCollider(PCollider2DScriptBase collider);
	void AddCollider(PCollider2DScriptBase collider);
	void AddRigidBody(PRigidBody2D rigidBody);

	void RemoveCollider(PCollider2DScriptBase collider);
	void RemoveRigidBody(PRigidBody2D rigidbody);

	static ColliderTypes* GetMap();

	static void CheckBoxWithBox(PCollider2DScriptBase alpha, PCollider2DScriptBase beta);
protected:
	void Unload();
protected:
	~Physic();
	static PPhysic __instance;
	
	static ColliderTypes* map;

	//std::list<PCollider2DScriptBase> tilemap;
	std::list<PRigidBody2D> rigidBodis;
	std::list<PCollider2DScriptBase> colliders;
};